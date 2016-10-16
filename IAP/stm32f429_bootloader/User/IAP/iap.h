#ifndef __IAP_H__
#define __IAP_H__
#include "stm32f4xx.h"


typedef  void (*iapfun)(void);				/*����һ���������͵Ĳ���. */  
#define FLASH_APP1_ADDR		0x0800C000  	/*��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)*/
											/*����0X08000000~0X0800BFFF�Ŀռ�ΪBootloaderʹ��(��49KB)*/	   
void iap_load_app(u32 appxaddr);			/*��ת��APP����ִ��*/
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	/*��ָ����ַ��ʼ,д��bin*/

#endif

