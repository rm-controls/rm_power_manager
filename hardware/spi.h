#ifndef POWER_MANAGER_HARDWARE_SPI_H_
#define POWER_MANAGER_HARDWARE_SPI_H_

#include "stm32h7xx_hal.h"
#include "system.h"

extern SPI_HandleTypeDef hspi3;

void spi3_config(void);

#endif /* POWER_MANAGER_HARDWARE_SPI_H_ */

