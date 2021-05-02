//
// Created by Lao·Zhu on 2021/5/2.
//

#ifndef PROGRAM_HARDWARE_SPI_H_
#define PROGRAM_HARDWARE_SPI_H_

#include "stm32h7xx_hal.h"

extern SPI_HandleTypeDef hspi1;
unsigned char SPI_ReadWriteByte(unsigned char Txdata);
void SPI_SetSpeed(unsigned int SPI_BaudRatePrescaler);
void SPI_Config(void);

#endif //PROGRAM_HARDWARE_SPI_H_
