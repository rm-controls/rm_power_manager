//
// Created by Lao·Zhu on 2021/4/29.
//

#include "eeprom.h"
#include "iic.h"
#include "system.h"

unsigned char EEPROM_ReadOneByte(unsigned char ReadAddr) {
    unsigned char temp;
    IIC_Start();
    IIC_SendByte(0xA0);
    Delayus(1);
    IIC_SendByte(ReadAddr);
    IIC_Start();
    IIC_SendByte(0xA0 | 0x01);
    Delayus(1);
    temp = IIC_ReadByte();
    IIC_Stop();
    return temp;
}

void EEPROM_WriteOneByte(unsigned char WriteAddr, unsigned char DataToWrite) {
    IIC_Start();
    IIC_SendByte(0xA0 | 0x00);
    Delayus(1);
    IIC_SendByte(WriteAddr);
    IIC_SendByte(DataToWrite);
    IIC_Stop();
    Delayus(10);
}

void EEPROM_Read(unsigned char ReadAddr, unsigned char *pBuffer, unsigned char NumToRead) {
    while (NumToRead) {
        *pBuffer++ = EEPROM_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

void EEPROM_Write(unsigned char WriteAddr, unsigned char *pBuffer, unsigned char NumToWrite) {
    while (NumToWrite--) {
        EEPROM_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}
