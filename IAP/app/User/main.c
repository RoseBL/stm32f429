/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   全彩LED灯例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./led/bsp_color_led.h"
#include "./usart/bsp_debug_usart.h"

static void Delay(__IO u32 nCount); 


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void) 
{

	uint32_t random_color = 0;
	
	SCB->VTOR = FLASH_BASE | 0xc000;/*设置偏移量*/


  /* 初始化呼吸灯 */
	ColorLED_Config(); 
	
  /*初始化串口*/
  Debug_USART_Config();
	
	/* 使能RNG时钟 */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);	
	/* 使能RNG外设 */
  RNG_Cmd(ENABLE);

 
	printf("\r\n 欢迎使用秉火  STM32 F429 开发板。\r\n");		
	printf("\r\n 全彩LED灯例程\r\n");	  
  printf("\r\n 使用PWM控制RGB灯，可控制输出各种颜色\r\n ");

  
  
  while(1)
  {
		
		SetRGBColor(COLOR_YELLOW); 
		Delay(0xFFFFFF); 
		
		/* 等待随机数产生完毕 */
		while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET);
		/*获取随机数*/       
		random_color = RNG_GetRandomNumber();
		
		printf("\r\n 随机颜色值：0x%06x",random_color&0xFFFFFF);
		/*显示随机颜色*/
		SetRGBColor(random_color&0xFFFFFF); 
		Delay(0x5FFFFFF); 



  }

}


static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

