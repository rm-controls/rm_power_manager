//
// Created by LaoZhu on 2023/6/6.
//

#include "iic.h"
#include "gpio.h"
#include "system.h"

#define IIC_SCL_PORT    LEFT_KEY_PORT
#define IIC_SCL_PIN     LEFT_KEY_PIN
#define IIC_SDA_PORT    RIGHT_KEY_PORT
#define IIC_SDA_PIN     RIGHT_KEY_PIN

void iic_config(void) {
    GPIO_InitTypeDef GPIO_Structure;

    GPIO_Structure.Pin = IIC_SDA_PIN;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IIC_SDA_PORT, &GPIO_Structure);

    GPIO_Structure.Pin = IIC_SCL_PIN;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IIC_SCL_PORT, &GPIO_Structure);

    HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
}

void iic_start(void) {
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
    delayus(5);
    HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET);
    delayus(5);
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
}

void iic_stop(void) {
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET);
    delayus(2);
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
    delayus(5);
    HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
    delayus(5);
}

void iic_send_ack(void) {
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET);
    delayus(5);
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
    delayus(5);
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
}

void iic_send_nack(void) {
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
    delayus(2);
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
    delayus(5);
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
}

unsigned char iic_wait_ack(void) {
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
    delayus(2);
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
    delayus(2);
    if (HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN) == GPIO_PIN_SET) {
        iic_stop();
        return 1;
    } else {
        HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
        return 0;
    }
}

void iic_send_byte(unsigned char data) {
    HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
    for (unsigned char counter = 0; counter < 8; counter++) {
        if (data & 0x80)
            HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET);
        data <<= 1;
        delayus(2);
        HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
        delayus(2);
        HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
    }
}

unsigned char iic_read_byte(void) {
    unsigned receive = 0;
    for (unsigned char counter = 0; counter < 8; counter++) {
        HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
        delayus(2);
        HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
        receive <<= 1;
        if (HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN) == GPIO_PIN_SET)
            receive++;
        delayus(2);
    }
    return receive;
}