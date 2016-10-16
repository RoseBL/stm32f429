/****************************************************
* FileName: bsp_internalFlash.c 
* Author: ����
* Description: �ڲ�FLASH��д���Է���
* Version: 1.0
* Date:    2016-10-16 
* FunctionList: 
* 1.-------
* History: // ��ʷ�޸ļ�¼
*       <author>    <time>  <version>    <desc>
* 
****************************************************/

  
#include "./internalFlash/bsp_internalFlash.h"   

#include "bsp_SysTick.h"

static uint32_t GetSector(uint32_t Address);

/*************************************************
* Function: FLASH_ReadWord
* Description: ���ڶ�ȡflash��ַ�����ֵ
* Input: faddr:Ҫ���ĵ�ַ
* Output: ��
* Return: �ڴ��е�ֵ
* Others: ��
*************************************************/

u32 FLASH_ReadWord(u32 faddr)
{
    return *(__IO uint32_t *)faddr; 
}

/*************************************************
* Function: STMFLASH_WriteWord
* Description: ��flashд��һ���ֽ�
* Input: faddr:д��ĵ�ַ
*        dat:д���ֵ
* Output: ��
* Return: 0
* Others: �ú������տ⺯��FLASH_ProgramWord()����������Ӧ
*         ���޸ģ�����һ����ʱ��Ҳ����һ������flash��ѹ��Χ
*************************************************/
u8 STMFLASH_WriteWord(u32 faddr, u32 dat)
{
	
    u8 res = 0;	 
    Delay_us(10);	 /*�ȴ��������,һ���ֱ��,���100us.*/
	
    FLASH->CR &= CR_PSIZE_MASK;  /*���PSIZEԭ��������*/
    FLASH->CR |= FLASH_PSIZE_WORD;  /*FLASH�����ڼ�,�����ֹ���ݻ���!*/
    FLASH->CR |= FLASH_CR_PG;  /*���ʹ��*/
    FLASH->CR |= VoltageRange_3;	/*����Ϊ32bit��,ȷ��VCC=2.7~3.6V֮��!!*/
    *(vu32*)faddr=dat;	/*д������*/
    FLASH->CR&=~(1<<0);  /*���PGλ.*/
   
	 
    return res;
} 

/*************************************************
* Function: STMFLASH_Write
* Description: ��ָ����ַ��ʼд��ָ�����ȵ�����
* Input: WriteAddr:д��ĵ�ַ
*        *pBuffer��д�������
*        NumToWrite��д�����ݵĴ�С
* Output: ��
* Return: void
* Others: ��
*************************************************/
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
    u8 status=0;
    u32 addrx=0;
    u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)/*�Ƿ���ַ*/
    {
        return;	
    }
    FLASH_Unlock();									/*���� */
    addrx=WriteAddr;				/*д�����ʼ��ַ*/
    endaddr=WriteAddr+NumToWrite*4;	/*д��Ľ�����ַ*/
    if(addrx<0X1FFF0000)			/*ֻ�����洢��,����Ҫִ�в�������!!*/
    {
        while(addrx<endaddr)		/*ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)*/
        {
            if(FLASH_ReadWord(addrx)!=0XFFFFFFFF)/*�з�0XFFFFFFFF�ĵط�,Ҫ�����������*/
            {   
                status=FLASH_EraseSector(GetSector(addrx), VoltageRange_3);
                if(status)
                {
                    break;
                }	
            }
            else 
            {
                addrx+=4;
            }
        } 
    }
    if(status==0)
    {
        while(WriteAddr<endaddr)/*д����*/
        {
            if(STMFLASH_WriteWord(WriteAddr,*pBuffer))/*д������*/
            { 
                break;	/*д���쳣*/
            }
            WriteAddr+=4;
            pBuffer++;
        } 
    }
    FLASH->ACR |= FLASH_ACR_ICEN;		/*FLASH��������,��������fetch*/
    FLASH_Lock();/*����*/
} 

/*************************************************
* Function: STMFLASH_Read
* Description: ��ָ����ַ��ʼ����ָ�����ȵ�����
* Input: ReadAddr:��ȡ��ַ
*        *pBuffer����ȡ������
*        NumToRead����ȡ���ݵĴ�С
* Output: ��
* Return: void
* Others: ��
*************************************************/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=FLASH_ReadWord(ReadAddr);/*��ȡ4���ֽ�.*/
        ReadAddr+=4;/*ƫ��4���ֽ�.*/	
    }

}	

 /*************************************************
* Function: GetSector
* Description: ��������ĵ�ַ���������ڵ�sector
* Input: Address:��ַ       
* Output: ��
* Return: ��ַ���ڵ�sector
* Others: ��
*************************************************/
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }

  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11;  
  }

  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_Sector_12;  
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_Sector_13;  
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_Sector_14;  
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_Sector_15;  
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_Sector_16;  
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_Sector_17;  
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_Sector_18;  
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_Sector_19;  
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_Sector_20;  
  } 
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_Sector_21;  
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_Sector_22;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_Sector_23;  
  }
  return sector;
}

