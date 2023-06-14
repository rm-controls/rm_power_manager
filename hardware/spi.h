#ifndef POWER_MANAGER_HARDWARE_SPI_H_
#define POWER_MANAGER_HARDWARE_SPI_H_

#include "stm32h7xx_hal.h"
#include "system.h"

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

void spi1_config(void);
void spi3_config(void);
void spi_setspeed(SPI_HandleTypeDef *hspi, unsigned int SPI_BaudRatePrescaler);

#endif /* POWER_MANAGER_HARDWARE_SPI_H_ */

