#ifndef POWER_MANAGER_HARDWARE_USART_H_
#define POWER_MANAGER_HARDWARE_USART_H_

#include "stm32h7xx_hal.h"
#include "system.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void usart1_send_error_package(unsigned char error_code, char *str);

void usart1_config(void);
void usart2_config(void);

#endif /* POWER_MANAGER_HARDWARE_USART_H_ */
