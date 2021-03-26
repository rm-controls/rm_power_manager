#ifndef __USART_H__
#define __USART_H__

#include "stm32h7xx_hal.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern unsigned char aRxBuffer1[1];
extern unsigned char aRxBuffer2[64];

void UART1_Config(void);
void UART2_Config(void);

#endif /* __USART_H__ */
