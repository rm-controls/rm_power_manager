#include "lcd.h"
#include "system.h"
#include "stm32h7xx_hal.h"
#include "port.h"

void LCD_WriteReg(unsigned int Com) {
    unsigned char i;
    HAL_GPIO_WritePin(LCD_A0_Port, LCD_A0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CSB_Port, LCD_CSB_Pin, GPIO_PIN_RESET);
    for (i = 0; i < 8; i++) {
        if (Com & 0x80UL)
            HAL_GPIO_WritePin(LCD_SDA_Port, LCD_SDA_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(LCD_SDA_Port, LCD_SDA_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LCD_SCL_Port, LCD_SCL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LCD_SCL_Port, LCD_SCL_Pin, GPIO_PIN_SET);
        Com <<= 1UL;
    }
    HAL_GPIO_WritePin(LCD_CSB_Port, LCD_CSB_Pin, GPIO_PIN_SET);
}
void LCD_WriteData(unsigned int dat) {
    unsigned char i;
    HAL_GPIO_WritePin(LCD_A0_Port, LCD_A0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CSB_Port, LCD_CSB_Pin, GPIO_PIN_RESET);
    for (i = 0; i < 8; i++) {
        if (dat & 0x80UL)
            HAL_GPIO_WritePin(LCD_SDA_Port, LCD_SDA_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(LCD_SDA_Port, LCD_SDA_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LCD_SCL_Port, LCD_SCL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LCD_SCL_Port, LCD_SCL_Pin, GPIO_PIN_SET);
        dat <<= 1UL;
    }
    HAL_GPIO_WritePin(LCD_CSB_Port, LCD_CSB_Pin, GPIO_PIN_SET);
}

void Lcd_Write_Data(unsigned int dat16) {
    LCD_WriteData(dat16 >> 8UL);
    LCD_WriteData(dat16);
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    x1 = x1 + 2;
    x2 = x2 + 2;
    y1 = y1 + 1;
    y2 = y2 + 1;
    LCD_WriteReg(0X2A);
    LCD_WriteData(x1 >> 8UL);
    LCD_WriteData(x1 & 0XFFUL);
    LCD_WriteData(x2 >> 8UL);
    LCD_WriteData(x2 & 0XFFUL);
    LCD_WriteReg(0X2BUL);
    LCD_WriteData(y1 >> 8UL);
    LCD_WriteData(y1 & 0XFFUL);
    LCD_WriteData(y2 >> 8UL);
    LCD_WriteData(y2 & 0XFFUL);
    LCD_WriteReg(0X2CUL);
}

void LCD_Scan(unsigned char mode) {
    switch (mode) {
        case 1: LCD_WriteReg(0x36);
            LCD_WriteData(0x08);
            LCD_WriteReg(0X2C);
            break;
        case 2: LCD_WriteReg(0x36);
            LCD_WriteData(0x88);
            LCD_WriteReg(0X2C);
            break;
        case 3: LCD_WriteReg(0x36);
            LCD_WriteData(0x48);
            LCD_WriteReg(0X2C);
            break;
        case 4: LCD_WriteReg(0x36);
            LCD_WriteData(0xc8);
            LCD_WriteReg(0X2C);
            break;
        case 5: LCD_WriteReg(0x36);
            LCD_WriteData(0x28);
            LCD_WriteReg(0X2C);
            break;
        case 6: LCD_WriteReg(0x36);
            LCD_WriteData(0x68);
            LCD_WriteReg(0X2C);
            break;
        case 7: LCD_WriteReg(0x36);
            LCD_WriteData(0xa8);
            LCD_WriteReg(0X2C);
            break;
        case 8: LCD_WriteReg(0x36);
            LCD_WriteData(0xe8);
            LCD_WriteReg(0X2C);
            break;
        default:break;
    }
}

void LCD_Config() {
    HAL_GPIO_WritePin(LCD_RES_Port, LCD_RES_Pin, GPIO_PIN_SET);
    Delayus(1000);
    HAL_GPIO_WritePin(LCD_RES_Port, LCD_RES_Pin, GPIO_PIN_RESET);
    Delayus(1000);
    HAL_GPIO_WritePin(LCD_RES_Port, LCD_RES_Pin, GPIO_PIN_SET);
    Delayus(120 * 1000);
    LCD_WriteReg(0x11);
    Delayus(120 * 1000);

    LCD_WriteReg(0xB1);
    LCD_WriteData(0x05);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x3C);
    LCD_WriteReg(0xB2);
    LCD_WriteData(0x05);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x3C);
    LCD_WriteReg(0xB3);
    LCD_WriteData(0x05);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x05);
    LCD_WriteData(0x3C);
    LCD_WriteData(0x3C);
    LCD_WriteReg(0xB4);
    LCD_WriteData(0x03);
    LCD_WriteReg(0xC0);
    LCD_WriteData(0x28);
    LCD_WriteData(0x08);
    LCD_WriteData(0x04);
    LCD_WriteReg(0xC1);
    LCD_WriteData(0XC0);
    LCD_WriteReg(0xC2);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x00);
    LCD_WriteReg(0xC3);
    LCD_WriteData(0x8D);
    LCD_WriteData(0x2A);
    LCD_WriteReg(0xC4);
    LCD_WriteData(0x8D);
    LCD_WriteData(0xEE);
    LCD_WriteReg(0xC5);
    LCD_WriteData(0x1A);
    LCD_WriteReg(0x36);
    LCD_WriteData(0xC0);
    LCD_WriteReg(0xE0);
    LCD_WriteData(0x04);
    LCD_WriteData(0x22);
    LCD_WriteData(0x07);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x2E);
    LCD_WriteData(0x30);
    LCD_WriteData(0x25);
    LCD_WriteData(0x2A);
    LCD_WriteData(0x28);
    LCD_WriteData(0x26);
    LCD_WriteData(0x2E);
    LCD_WriteData(0x3A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x03);
    LCD_WriteData(0x13);
    LCD_WriteReg(0xE1);
    LCD_WriteData(0x04);
    LCD_WriteData(0x16);
    LCD_WriteData(0x06);
    LCD_WriteData(0x0D);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x26);
    LCD_WriteData(0x23);
    LCD_WriteData(0x27);
    LCD_WriteData(0x27);
    LCD_WriteData(0x25);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x3B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x04);
    LCD_WriteData(0x13);
    LCD_WriteReg(0x3A);
    LCD_WriteData(0x05);
    LCD_WriteReg(0x29);
}
