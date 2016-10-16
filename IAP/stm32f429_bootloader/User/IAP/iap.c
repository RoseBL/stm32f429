/****************************************************
* FileName:iap.c 
* Author: 刘斌
* Description: 该函数用于把串口存放的程序写到flash，并调到app程序运行
* Version: 1.0
* Date:    2016-10-16 
* FunctionList: 
* 1.-------
* History: // 历史修改记录
* 		<author> 	<time> 	<version>	 <desc>
* 
****************************************************/


#include "iap.h"
#include "./usart/bsp_debug_usart.h"
#include "./internalFlash/bsp_internalFlash.h"

iapfun jump2app; 
u32 iapbuf[512]; 	/*2K字节缓存*/  


/*************************************************
* Function: MSR_MSP
* Description: 设置栈顶地址
* Input: addr:栈顶地址
* Output: 无
* Return: void
* Others: 无
*************************************************/
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 	//set Main Stack value
	BX r14
}


/*************************************************
* Function: iap_write_appbin
* Description: 将存放在串口接收buf里面的APP程序写入到FLASH
* Input: appxaddr:应用程序的起始地址
*        appbuf:应用程序CODE.
*        appsize:应用程序大小(字节).
* Output: 无
* Return: void
* Others: 无
*************************************************/
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u32 t;
	u16 i=0;
	u32 temp;
	u32 fwaddr=appxaddr;/*当前写入的地址*/
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{						   
		temp = (u32)dfu[3]<<24;   
		temp |= (u32)dfu[2]<<16;    
		temp |= (u32)dfu[1]<<8;
		temp |= (u32)dfu[0];	  
		dfu += 4;/*偏移4个字节*/
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			STMFLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;/*偏移2048  512*4=2048*/
		}
	} 
	if(i)
	{
		STMFLASH_Write(fwaddr,iapbuf,i);/*将最后的一些内容字节写进去.*/
	} 
}


/*************************************************
* Function: iap_load_app
* Description: 用于跳转到APPA程序运行
* Input: appxaddr:用户代码起始地址.
* Output: 无
* Return: void
* Others: 无
*************************************************/
void iap_load_app(u32 appxaddr)
{ 
	if(((*(vu32*)appxaddr)&0x2FF00000)==0x20000000)	/*检查栈顶地址是否合法.*/
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		/*用户代码区第二个字为程序开始地址(复位地址)*/

		MSR_MSP(*(vu32*)appxaddr);					/*初始化APP堆栈指针*/
                                                    /*(用户代码区的第一个字用于存放栈顶地址)*/

		jump2app();									/*跳转到APP.*/
	}
}		 




