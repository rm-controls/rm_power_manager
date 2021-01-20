#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart1;
extern unsigned char aRxBuffer[128];
void UART_Config(void);

#endif /* __USART_H__ */
