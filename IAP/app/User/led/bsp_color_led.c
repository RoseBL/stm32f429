/**
  ******************************************************************************
  * @file    bsp_breath_led.c
  * @author  STMicroelectronics
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
  
#include "./led/bsp_color_led.h"


 /**
  * @brief  ����TIM3�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( COLOR_LED1_GPIO_CLK|COLOR_LED2_GPIO_CLK|COLOR_LED3_GPIO_CLK|LED4_GPIO_CLK, ENABLE); 

		GPIO_PinAFConfig(COLOR_LED1_GPIO_PORT,COLOR_LED1_PINSOURCE,COLOR_LED1_AF); 
		GPIO_PinAFConfig(COLOR_LED2_GPIO_PORT,COLOR_LED2_PINSOURCE,COLOR_LED2_AF); 
		GPIO_PinAFConfig(COLOR_LED3_GPIO_PORT,COLOR_LED3_PINSOURCE,COLOR_LED3_AF);  
	
		/*COLOR_LED1*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_LED1_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_Init(COLOR_LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*COLOR_LED2*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_LED2_PIN;	
    GPIO_Init(COLOR_LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*COLOR_LED3*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_LED3_PIN;	
    GPIO_Init(COLOR_LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*LED4*/	
		/*��������ģʽΪ���ģʽ LED4���Ƕ�ʱ����ͨ��������ʹ��pwm���ƣ����Բ�������������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  		
		GPIO_InitStructure.GPIO_Pin = LED4_PIN;	
    GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);	
		
//		/*�ر�RGB��*/
//		LED_RGBOFF;
		
		/*ָʾ��Ĭ�Ͽ���*/
		LED4(ON);

}



/*
 * TIM_Period / Auto Reload Register(ARR) = 256-1   TIM_Prescaler=((SystemCoreClock/2)/1000000)*30-1 
 *  *	
 * ��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
 *				PCLK1 = HCLK / 4 
 *				=> TIMxCLK = HCLK / 2 = SystemCoreClock /2
 * ��ʱ��Ƶ��Ϊ = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock /2)/((SystemCoreClock/2)/1000000)*30 = 1000000/30 = 1/30MHz
 * �ж�����Ϊ = 1/(1/30MHz) * 256 = x ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8,15,16,17]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	// ����TIMx_CLK,x[2,3,4,5] 
  RCC_APB1PeriphClockCmd(COLOR_TIM_CLK, ENABLE); 
	
  /* �ۼ� TIM_Period�������һ�����»����ж�*/		 
  TIM_TimeBaseStructure.TIM_Period = 256-1;       //����ʱ����0������255����Ϊ256�Σ�Ϊһ����ʱ����
	
	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
  //				PCLK1 = HCLK / 4 
  //				=> TIMxCLK = HCLK / 2 = SystemCoreClock /2
	// ��ʱ��Ƶ��Ϊ = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock /2)/((SystemCoreClock/2)/1000000)*30 = 1000000/30 = 1/30MHz
  /*������ʱ������TIM_Prescaler����Ч�������ü��ɣ��ж�����С ����˸�죬������˸����*/
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/2)/1000000)*30-1;	    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  	
	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(COLOR_TIM, &TIM_TimeBaseStructure);	
	
	
	/*PWMģʽ����*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    			//����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;	  
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ LED����
  TIM_OC1Init(COLOR_TIM, &TIM_OCInitStructure);	 							//ʹ��ͨ��1
  
	/*ʹ��ͨ��1����*/
	TIM_OC1PreloadConfig(COLOR_TIM, TIM_OCPreload_Enable);
	
	
	TIM_OC2Init(COLOR_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��2  
	/*ʹ��ͨ��2����*/
	TIM_OC2PreloadConfig(COLOR_TIM, TIM_OCPreload_Enable);


  TIM_OC3Init(COLOR_TIM, &TIM_OCInitStructure);	 //ʹ��ͨ��3  
	/*ʹ��ͨ��3����*/
	TIM_OC3PreloadConfig(COLOR_TIM, TIM_OCPreload_Enable);


	TIM_ARRPreloadConfig(COLOR_TIM, ENABLE);			//ʹ��TIM���ؼĴ���ARR
	
	// ʹ�ܼ�����
	TIM_Cmd(COLOR_TIM, ENABLE);																		


}


//RGBLED��ʾ��ɫ,24λ��RGB��ɫֵ
void SetRGBColor(uint32_t rgb)
{
	uint8_t r=0,g=0,b=0;
	r=(uint8_t)(rgb>>16);
	g=(uint8_t)(rgb>>8);
	b=(uint8_t)rgb;
	COLOR_TIM->COLOR_LED1_CCRx = r;	//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
	COLOR_TIM->COLOR_LED2_CCRx = g;	//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ        
	COLOR_TIM->COLOR_LED3_CCRx = b;	//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
}

//RGBLED��ʾ��ɫ����8λ��R G B����
void SetColorValue(uint8_t r,uint8_t g,uint8_t b)
{
	COLOR_TIM->COLOR_LED1_CCRx = r;	//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
	COLOR_TIM->COLOR_LED2_CCRx = g;	//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ        
	COLOR_TIM->COLOR_LED3_CCRx = b;	//����PWM���޸Ķ�ʱ���ıȽϼĴ���ֵ
}


/**
  * @brief  ��ʼ��������
  * @param  ��
  * @retval ��
  */
void ColorLED_Config(void)
{

	TIMx_GPIO_Config();
	
	TIM_Mode_Config();
	
	//�ϵ��Ĭ����ʾ
	SetColorValue(0xff,0xff,0xff);

}

/*********************************************END OF FILE**********************/
