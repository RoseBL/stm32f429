#ifndef __COLOR_LED_H
#define	__COLOR_LED_H

#include "stm32f4xx.h"

//���Ŷ���
/*******************************************************/
//R ��ɫ��
#define COLOR_LED1_PIN                  GPIO_Pin_10                 
#define COLOR_LED1_GPIO_PORT            GPIOH                      
#define COLOR_LED1_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define COLOR_LED1_PINSOURCE						GPIO_PinSource10
#define COLOR_LED1_AF										GPIO_AF_TIM5



//G ��ɫ��
#define COLOR_LED2_PIN                  GPIO_Pin_11                 
#define COLOR_LED2_GPIO_PORT            GPIOH                      
#define COLOR_LED2_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define COLOR_LED2_PINSOURCE						GPIO_PinSource11
#define COLOR_LED2_AF										GPIO_AF_TIM5

//B ��ɫ��
#define COLOR_LED3_PIN                  GPIO_Pin_12                 
#define COLOR_LED3_GPIO_PORT            GPIOH                       
#define COLOR_LED3_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define COLOR_LED3_PINSOURCE						GPIO_PinSource12
#define COLOR_LED3_AF										GPIO_AF_TIM5

//Сָʾ�ƣ���ͨIO�ڣ�������PWM����
#define LED4_PIN                  GPIO_Pin_11                 
#define LED4_GPIO_PORT            GPIOD                       
#define LED4_GPIO_CLK             RCC_AHB1Periph_GPIOD



/*��ʱ��*/

#define COLOR_TIM           		TIM5
#define COLOR_TIM_CLK       		RCC_APB1Periph_TIM5

#define COLOR_LED1_CCRx								CCR1
#define COLOR_LED2_CCRx								CCR2
#define COLOR_LED3_CCRx								CCR3

#define COLOR_LED1_TIM_CHANNEL				TIM_Channel_1
#define COLOR_LED2_TIM_CHANNEL				TIM_Channel_2
#define COLOR_LED3_TIM_CHANNEL				TIM_Channel_3

#define COLOR_TIM_IRQn					TIM5_IRQn
#define COLOR_TIM_IRQHandler 	TIM5_IRQHandler
/************************************************************/


/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  0
#define OFF 1

/* ���κ꣬��������������һ��ʹ�� */
		
//LED 4����ͨIO��					
#define LED4(a)	if (a)	\
					GPIO_SetBits(LED4_GPIO_PORT,LED4_PIN);\
					else		\
					GPIO_ResetBits(LED4_GPIO_PORT,LED4_PIN)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRRL=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRRH=i;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬

/* �������IO�ĺ� */
#define LED4_TOGGLE		digitalToggle(LED4_GPIO_PORT,LED4_PIN)
#define LED4_OFF				digitalHi(LED4_GPIO_PORT,LED4_PIN)
#define LED4_ON				digitalLo(LED4_GPIO_PORT,LED4_PIN)


/* RGB��ɫֵ 24λ*/
#define COLOR_WHITE          0xFFFFFF
#define COLOR_BLACK          0x000000
#define COLOR_GREY           0xC0C0C0
#define COLOR_BLUE           0x0000FF
#define COLOR_RED            0xFF0000
#define COLOR_MAGENTA        0xFF00FF
#define COLOR_GREEN          0x00FF00
#define COLOR_CYAN           0x00FFFF
#define COLOR_YELLOW         0xFFFF00


void ColorLED_Config(void);
void SetRGBColor(uint32_t rgb);
void SetColorValue(uint8_t r,uint8_t g,uint8_t b);

#endif /* __COLOR_LED_H */
