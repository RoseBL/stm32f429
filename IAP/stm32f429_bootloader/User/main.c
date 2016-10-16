/****************************************************
* FileName:main.c 
* Author: 刘斌
* Description: 主要用于接受接受串口发的的app程序，写入到flash中，最后执行。
* Version: 1.0
* Date:    2016-10-16 
* FunctionList: 
* 1.-------
* History: // 历史修改记录
* 		<author> 	<time> 	<version>	 <desc>
* 
****************************************************/

  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"   
#include "./internalFlash/bsp_internalFlash.h"   
#include "iap.h"
#include "bsp_SysTick.h"


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	
    u16 oldcount=0;				    /*老的串口接收数据值*/
    u32 applenth=0;				    /*接收到的app代码长度*/
		
    Debug_USART_Config();
	
    SysTick_Init();
	
    LED_GPIO_Config();
    while (1)
    {
        if (USART_RX_CNT)
        {
			
            if (oldcount==USART_RX_CNT)/*新周期内,没有收到任何数据,认为本次数据接收完成.*/
            {
                applenth=USART_RX_CNT;
                oldcount=0;
                USART_RX_CNT=0;
                printf("用户程序接收完成!\r\n");
                printf("代码长度:%dBytes\r\n",applenth);
            }
            else
            {
                oldcount=USART_RX_CNT;
            }			
        }
			
			Delay_ms(10);
			
	//if(key==WKUP_PRES)	/*WK_UP按键按下*/
       {
            if(applenth)
            {
                printf("开始更新固件...\r\n");	
				
                if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)/*判断是否为0X08XXXXXX.*/
                {	 
                    iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);/*更新FLASH代码*/   
					
                    printf("固件更新完成!\r\n");	
                }
                else 
                {
					   
                    printf("非FLASH应用程序!\r\n");
                }
            }
            else 
            {
                printf("没有可以更新的固件!\r\n");
            }
								 
        }
				
		//if(key==KEY2_PRES)	//KEY2按下
        {
            printf("开始执行FLASH用户代码!!\r\n");
            if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)/*判断是否为0X08XXXXXX.*/
            {	 						
                iap_load_app(FLASH_APP1_ADDR);/*执行FLASH APP代码*/
            }
            else 
            {
                printf("非FLASH应用程序,无法执行!\r\n");
				   
            }									   
        }				
    }
}

/*********************************************END OF FILE**********************/

