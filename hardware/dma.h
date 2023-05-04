#ifndef POWER_MANAGER_HARDWARE_DMA_H_
#define POWER_MANAGER_HARDWARE_DMA_H_

#include "stm32h7xx_hal.h"
#include "system.h"

extern MDMA_HandleTypeDef hmdma_referee;
extern unsigned char power_manager_status_send_flag, mdma_status_flag;

void dma_config(void);
void mdma_transmit_buffer(const unsigned char *source, int length);

#endif /* POWER_MANAGER_HARDWARE_DMA_H_ */

