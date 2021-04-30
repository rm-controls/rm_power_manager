//
// Created by Lao·Zhu on 2021/4/29.
//

#include "iic.h"
#include "stm32h7xx_hal.h"
#include "system.h"

void IIC_Stop(void) {
    SDA_OUT();
    IIC_SDA(0);
    Delayus(2);
    IIC_SCL(1);
    Delayus(2);
    IIC_SDA(1);
    Delayus(2);
}

void IIC_Start(void) {
    SDA_OUT();
    IIC_SCL(1);
    IIC_SDA(1);
    Delayus(2);
    IIC_SDA(0);
    Delayus(2);
    IIC_SCL(0);
    Delayus(2);
}

unsigned char IIC_Wait_Ack(void) {
    unsigned char ack;
    SDA_IN();
    Delayus(1);
    IIC_SCL(1);
    Delayus(1);
    if (READ_SDA == GPIO_PIN_RESET)
        ack = 0;
    else
        ack = 1;
    return ack;
}

unsigned char IIC_SendByte(uint8_t IIC_Byte) {
    unsigned char i;
    SDA_OUT();
    for (i = 0; i < 8; i++) {
        IIC_SCL(0);
        Delayus(2);
        if (IIC_Byte & 0x80)
            IIC_SDA(1);
        else
            IIC_SDA(0);
        IIC_Byte <<= 1;
        Delayus(2);
        IIC_SCL(1);
        Delayus(2);
    }
    IIC_SCL(0);
    Delayus(2);
    i = 0;
    while (1) {
        if (IIC_Wait_Ack() == 0)
            return 0;
        i++;
        if (i == 10)
            return 1;
        Delayus(1);
    }
}

uint8_t IIC_ReadByte(void) {
    uint8_t rebyte = 0, i;
    SDA_IN();
    IIC_SCL(0);
    Delayus(2);
    for (i = 0; i < 8; i++) {
        IIC_SCL(1);
        Delayus(2);
        rebyte = rebyte << 1;
        if (READ_SDA == GPIO_PIN_SET) {
            rebyte |= 0x01;
        }
        IIC_SCL(0);
        Delayus(2);
    }
    SDA_OUT();
    IIC_SDA(1);
    Delayus(2);
    IIC_SCL(1);
    Delayus(2);
    IIC_SCL(0);
    return rebyte;
}
