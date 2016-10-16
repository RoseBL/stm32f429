/****************************************************
* FileName:main.c 
* Author: ����
* Description: ��Ҫ���ڽ��ܽ��ܴ��ڷ��ĵ�app����д�뵽flash�У����ִ�С�
* Version: 1.0
* Date:    2016-10-16 
* FunctionList: 
* 1.-------
* History: // ��ʷ�޸ļ�¼
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	
    u16 oldcount=0;				    /*�ϵĴ��ڽ�������ֵ*/
    u32 applenth=0;				    /*���յ���app���볤��*/
		
    Debug_USART_Config();
	
    SysTick_Init();
	
    LED_GPIO_Config();
    while (1)
    {
        if (USART_RX_CNT)
        {
			
            if (oldcount==USART_RX_CNT)/*��������,û���յ��κ�����,��Ϊ�������ݽ������.*/
            {
                applenth=USART_RX_CNT;
                oldcount=0;
                USART_RX_CNT=0;
                printf("�û�����������!\r\n");
                printf("���볤��:%dBytes\r\n",applenth);
            }
            else
            {
                oldcount=USART_RX_CNT;
            }			
        }
			
			Delay_ms(10);
			
	//if(key==WKUP_PRES)	/*WK_UP��������*/
       {
            if(applenth)
            {
                printf("��ʼ���¹̼�...\r\n");	
				
                if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)/*�ж��Ƿ�Ϊ0X08XXXXXX.*/
                {	 
                    iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);/*����FLASH����*/   
					
                    printf("�̼��������!\r\n");	
                }
                else 
                {
					   
                    printf("��FLASHӦ�ó���!\r\n");
                }
            }
            else 
            {
                printf("û�п��Ը��µĹ̼�!\r\n");
            }
								 
        }
				
		//if(key==KEY2_PRES)	//KEY2����
        {
            printf("��ʼִ��FLASH�û�����!!\r\n");
            if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)/*�ж��Ƿ�Ϊ0X08XXXXXX.*/
            {	 						
                iap_load_app(FLASH_APP1_ADDR);/*ִ��FLASH APP����*/
            }
            else 
            {
                printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
				   
            }									   
        }				
    }
}

/*********************************************END OF FILE**********************/

