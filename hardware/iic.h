//
// Created by Lao·Zhu on 2021/4/29.
//

#ifndef PROGRAM_HARDWARE_IIC_H_
#define PROGRAM_HARDWARE_IIC_H_

#include "port.h"

#define SDA_IN()  {IIC_SDA_PORT->MODER &= ~(3 << (12 * 2));IIC_SDA_PORT->MODER |= 0 << 12 * 2;}
#define SDA_OUT() {IIC_SDA_PORT->MODER &= ~(3 << (12 * 2));IIC_SDA_PORT->MODER |= 1 << 12 * 2;}

#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(IIC_SCL_PORT,IIC_SCL_PIN,GPIO_PIN_RESET))
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(IIC_SDA_PORT,IIC_SDA_PIN,GPIO_PIN_RESET))
#define READ_SDA    HAL_GPIO_ReadPin(IIC_SDA_PORT,IIC_SDA_PIN)

void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_SendByte(unsigned char IIC_Byte);
unsigned char IIC_ReadByte(void);

#endif //PROGRAM_HARDWARE_IIC_H_
