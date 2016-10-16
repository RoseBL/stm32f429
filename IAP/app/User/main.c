/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ȫ��LED������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./led/bsp_color_led.h"
#include "./usart/bsp_debug_usart.h"

static void Delay(__IO u32 nCount); 


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void) 
{

	uint32_t random_color = 0;
	
	SCB->VTOR = FLASH_BASE | 0xc000;/*����ƫ����*/


  /* ��ʼ�������� */
	ColorLED_Config(); 
	
  /*��ʼ������*/
  Debug_USART_Config();
	
	/* ʹ��RNGʱ�� */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);	
	/* ʹ��RNG���� */
  RNG_Cmd(ENABLE);

 
	printf("\r\n ��ӭʹ�ñ���  STM32 F429 �����塣\r\n");		
	printf("\r\n ȫ��LED������\r\n");	  
  printf("\r\n ʹ��PWM����RGB�ƣ��ɿ������������ɫ\r\n ");

  
  
  while(1)
  {
		
		SetRGBColor(COLOR_YELLOW); 
		Delay(0xFFFFFF); 
		
		/* �ȴ������������� */
		while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET);
		/*��ȡ�����*/       
		random_color = RNG_GetRandomNumber();
		
		printf("\r\n �����ɫֵ��0x%06x",random_color&0xFFFFFF);
		/*��ʾ�����ɫ*/
		SetRGBColor(random_color&0xFFFFFF); 
		Delay(0x5FFFFFF); 



  }

}


static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

