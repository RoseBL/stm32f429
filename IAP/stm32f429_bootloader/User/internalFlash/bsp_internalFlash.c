/****************************************************
* FileName: bsp_internalFlash.c 
* Author: 刘斌
* Description: 内部FLASH读写测试范例
* Version: 1.0
* Date:    2016-10-16 
* FunctionList: 
* 1.-------
* History: // 历史修改记录
*       <author>    <time>  <version>    <desc>
* 
****************************************************/

  
#include "./internalFlash/bsp_internalFlash.h"   

#include "bsp_SysTick.h"

static uint32_t GetSector(uint32_t Address);

/*************************************************
* Function: FLASH_ReadWord
* Description: 用于读取flash地址里面的值
* Input: faddr:要读的地址
* Output: 无
* Return: 内存中的值
* Others: 无
*************************************************/

u32 FLASH_ReadWord(u32 faddr)
{
    return *(__IO uint32_t *)faddr; 
}

/*************************************************
* Function: STMFLASH_WriteWord
* Description: 往flash写入一个字节
* Input: faddr:写入的地址
*        dat:写入的值
* Output: 无
* Return: 0
* Others: 该函数仿照库函数FLASH_ProgramWord()，并做了相应
*         的修改，加了一个延时，也加了一个设置flash电压范围
*************************************************/
u8 STMFLASH_WriteWord(u32 faddr, u32 dat)
{
	
    u8 res = 0;	 
    Delay_us(10);	 /*等待操作完成,一个字编程,最多100us.*/
	
    FLASH->CR &= CR_PSIZE_MASK;  /*清除PSIZE原来的设置*/
    FLASH->CR |= FLASH_PSIZE_WORD;  /*FLASH擦除期间,必须禁止数据缓存!*/
    FLASH->CR |= FLASH_CR_PG;  /*编程使能*/
    FLASH->CR |= VoltageRange_3;	/*设置为32bit宽,确保VCC=2.7~3.6V之间!!*/
    *(vu32*)faddr=dat;	/*写入数据*/
    FLASH->CR&=~(1<<0);  /*清除PG位.*/
   
	 
    return res;
} 

/*************************************************
* Function: STMFLASH_Write
* Description: 从指定地址开始写入指定长度的数据
* Input: WriteAddr:写入的地址
*        *pBuffer：写入的数据
*        NumToWrite：写入数据的大小
* Output: 无
* Return: void
* Others: 无
*************************************************/
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
    u8 status=0;
    u32 addrx=0;
    u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)/*非法地址*/
    {
        return;	
    }
    FLASH_Unlock();									/*解锁 */
    addrx=WriteAddr;				/*写入的起始地址*/
    endaddr=WriteAddr+NumToWrite*4;	/*写入的结束地址*/
    if(addrx<0X1FFF0000)			/*只有主存储区,才需要执行擦除操作!!*/
    {
        while(addrx<endaddr)		/*扫清一切障碍.(对非FFFFFFFF的地方,先擦除)*/
        {
            if(FLASH_ReadWord(addrx)!=0XFFFFFFFF)/*有非0XFFFFFFFF的地方,要擦除这个扇区*/
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
        while(WriteAddr<endaddr)/*写数据*/
        {
            if(STMFLASH_WriteWord(WriteAddr,*pBuffer))/*写入数据*/
            { 
                break;	/*写入异常*/
            }
            WriteAddr+=4;
            pBuffer++;
        } 
    }
    FLASH->ACR |= FLASH_ACR_ICEN;		/*FLASH擦除结束,开启数据fetch*/
    FLASH_Lock();/*上锁*/
} 

/*************************************************
* Function: STMFLASH_Read
* Description: 从指定地址开始读出指定长度的数据
* Input: ReadAddr:读取地址
*        *pBuffer：读取的数据
*        NumToRead：读取数据的大小
* Output: 无
* Return: void
* Others: 无
*************************************************/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=FLASH_ReadWord(ReadAddr);/*读取4个字节.*/
        ReadAddr+=4;/*偏移4个字节.*/	
    }

}	

 /*************************************************
* Function: GetSector
* Description: 根据输入的地址给出它所在的sector
* Input: Address:地址       
* Output: 无
* Return: 地址所在的sector
* Others: 无
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

