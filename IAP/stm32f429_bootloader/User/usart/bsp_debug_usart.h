#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>



//���Ŷ���
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART1

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource9

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 				USART1_IRQn
/************************************************************/


//���ڲ�����
#define DEBUG_USART_BAUDRATE                    115200



#define USART_REC_LEN  			180*1024/*�����������ֽ��� 180K*/
 	
extern u8  USART_RX_BUF[USART_REC_LEN]; /*���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�*/ 
extern u16 USART_RX_STA;         		/*����״̬���*/	

extern u32 USART_RX_CNT;				/*���յ��ֽ���*/	






void Debug_USART_Config(void);
int fputc(int ch, FILE *f);

#endif /* __USART1_H */
