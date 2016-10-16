/****************************************************
* FileName:iap.c 
* Author: ����
* Description: �ú������ڰѴ��ڴ�ŵĳ���д��flash��������app��������
* Version: 1.0
* Date:    2016-10-16 
* FunctionList: 
* 1.-------
* History: // ��ʷ�޸ļ�¼
* 		<author> 	<time> 	<version>	 <desc>
* 
****************************************************/


#include "iap.h"
#include "./usart/bsp_debug_usart.h"
#include "./internalFlash/bsp_internalFlash.h"

iapfun jump2app; 
u32 iapbuf[512]; 	/*2K�ֽڻ���*/  


/*************************************************
* Function: MSR_MSP
* Description: ����ջ����ַ
* Input: addr:ջ����ַ
* Output: ��
* Return: void
* Others: ��
*************************************************/
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 	//set Main Stack value
	BX r14
}


/*************************************************
* Function: iap_write_appbin
* Description: ������ڴ��ڽ���buf�����APP����д�뵽FLASH
* Input: appxaddr:Ӧ�ó������ʼ��ַ
*        appbuf:Ӧ�ó���CODE.
*        appsize:Ӧ�ó����С(�ֽ�).
* Output: ��
* Return: void
* Others: ��
*************************************************/
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u32 t;
	u16 i=0;
	u32 temp;
	u32 fwaddr=appxaddr;/*��ǰд��ĵ�ַ*/
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{						   
		temp = (u32)dfu[3]<<24;   
		temp |= (u32)dfu[2]<<16;    
		temp |= (u32)dfu[1]<<8;
		temp |= (u32)dfu[0];	  
		dfu += 4;/*ƫ��4���ֽ�*/
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			STMFLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;/*ƫ��2048  512*4=2048*/
		}
	} 
	if(i)
	{
		STMFLASH_Write(fwaddr,iapbuf,i);/*������һЩ�����ֽ�д��ȥ.*/
	} 
}


/*************************************************
* Function: iap_load_app
* Description: ������ת��APPA��������
* Input: appxaddr:�û�������ʼ��ַ.
* Output: ��
* Return: void
* Others: ��
*************************************************/
void iap_load_app(u32 appxaddr)
{ 
	if(((*(vu32*)appxaddr)&0x2FF00000)==0x20000000)	/*���ջ����ַ�Ƿ�Ϸ�.*/
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		/*�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)*/

		MSR_MSP(*(vu32*)appxaddr);					/*��ʼ��APP��ջָ��*/
                                                    /*(�û��������ĵ�һ�������ڴ��ջ����ַ)*/

		jump2app();									/*��ת��APP.*/
	}
}		 




