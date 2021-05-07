//
// Created by Lao·Zhu on 2021/5/3.
//

#include "flash.h"
#include "spi.h"
#include "system.h"

#define _nop() __asm__ __volatile__ ("nop"::)
#define FLASH_DELAY() _nop(); _nop(); _nop(); _nop(); _nop(); _nop()

unsigned short W25QXX_ID = 0x0000;
unsigned char W25QXX_BUFFER[4096];

void W25QXX_Init(void) {
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    W25QXX_WAKEUP();
    W25QXX_ID = W25QXX_ReadID();
}

unsigned char W25QXX_ReadSR(unsigned char regno) {
    unsigned char byte = 0, command = 0;
    switch (regno) {
        case 1:command = W25X_ReadStatusReg1;
            break;
        case 2:command = W25X_ReadStatusReg2;
            break;
        case 3:command = W25X_ReadStatusReg3;
            break;
        default:command = W25X_ReadStatusReg1;
            break;
    }
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(command);
    byte = SPI_ReadWriteByte(0Xff);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    return byte;
}

void W25QXX_Write_SR(unsigned char regno, unsigned char sr) {
    unsigned char command = 0;
    switch (regno) {
        case 1:command = W25X_WriteStatusReg1;
            break;
        case 2:command = W25X_WriteStatusReg2;
            break;
        case 3:command = W25X_WriteStatusReg3;
            break;
        default:command = W25X_WriteStatusReg1;
            break;
    }
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(command);
    SPI_ReadWriteByte(sr);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
}

void W25QXX_Write_Enable(void) {
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(W25X_WriteEnable);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
}

void W25QXX_Write_Disable(void) {
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(W25X_WriteDisable);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
}

unsigned short W25QXX_ReadID(void) {
    unsigned short Temp = 0;
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(0x90);
    SPI_ReadWriteByte(0x00);
    SPI_ReadWriteByte(0x00);
    SPI_ReadWriteByte(0x00);
    Temp |= SPI_ReadWriteByte(0xFF) << 8;
    Temp |= SPI_ReadWriteByte(0xFF);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    return Temp;
}

void W25QXX_Read(unsigned char *pBuffer, unsigned int ReadAddr, unsigned short NumByteToRead) {
    unsigned short i;
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(W25X_ReadData);
    SPI_ReadWriteByte((unsigned char) ((ReadAddr) >> 16));
    SPI_ReadWriteByte((unsigned char) ((ReadAddr) >> 8));
    SPI_ReadWriteByte((unsigned char) ReadAddr);
    for (i = 0; i < NumByteToRead; i++)
        pBuffer[i] = SPI_ReadWriteByte(0XFF);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
}

void W25QXX_Write_Page(unsigned char *pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite) {
    unsigned short i;
    W25QXX_Write_Enable();
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(W25X_PageProgram);
    SPI_ReadWriteByte((unsigned char) ((WriteAddr) >> 16));
    SPI_ReadWriteByte((unsigned char) ((WriteAddr) >> 8));
    SPI_ReadWriteByte((unsigned char) WriteAddr);
    for (i = 0; i < NumByteToWrite; i++)
        SPI_ReadWriteByte(pBuffer[i]);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    W25QXX_Wait_Busy();
}

void W25QXX_Write_NoCheck(unsigned char *pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite) {
    unsigned short pageremain;
    pageremain = 256 - WriteAddr % 256;
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite;
    while (1) {
        W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
        if (NumByteToWrite == pageremain)
            break;
        else {
            pBuffer += pageremain;
            WriteAddr += pageremain;
            NumByteToWrite -= pageremain;
            if (NumByteToWrite > 256)
                pageremain = 256;
            else
                pageremain = NumByteToWrite;
        }
    }
}

void W25QXX_Write(unsigned char *pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite) {
    unsigned int secpos;
    unsigned short secoff;
    unsigned short secremain;
    unsigned short i;
    unsigned char *W25QXX_BUF;
    for (unsigned short counter = 0; counter < 4096; ++counter)
        W25QXX_BUFFER[counter] = 0x00;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096;
    secoff = WriteAddr % 4096;
    secremain = 4096 - secoff;
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite;
    while (1) {
        W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096);
        for (i = 0; i < secremain; i++)
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break;
        if (i < secremain) {
            W25QXX_Erase_Sector(secpos);
            for (i = 0; i < secremain; i++)
                W25QXX_BUF[i + secoff] = pBuffer[i];
            W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096);

        } else
            W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);
        if (NumByteToWrite == secremain)
            break;
        else {
            secpos++;
            secoff = 0;
            pBuffer += secremain;
            WriteAddr += secremain;
            NumByteToWrite -= secremain;
            if (NumByteToWrite > 4096)
                secremain = 4096;
            else
                secremain = NumByteToWrite;
        }
    }
}

void W25QXX_Erase_Chip(void) {
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    W25QXX_CS_LOW;FLASH_DELAY();
    SPI_ReadWriteByte(W25X_ChipErase);
    FLASH_DELAY(); W25QXX_CS_HIGH;
    W25QXX_Wait_Busy();
}

void W25QXX_Erase_Sector(unsigned int Dst_Addr) {
    Dst_Addr *= 4096;
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(W25X_SectorErase);
    SPI_ReadWriteByte((unsigned char) ((Dst_Addr) >> 16));
    SPI_ReadWriteByte((unsigned char) ((Dst_Addr) >> 8));
    SPI_ReadWriteByte((unsigned char) Dst_Addr);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    W25QXX_Wait_Busy();
}

void W25QXX_Wait_Busy(void) {
    while ((W25QXX_ReadSR(1) & 0x01));
}

void W25QXX_PowerDown(void) {
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(W25X_PowerDown);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    Delayus(3);
}

void W25QXX_WAKEUP(void) {
    W25QXX_CS_LOW;
    FLASH_DELAY();
    SPI_ReadWriteByte(W25X_ReleasePowerDown);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    Delayus(3);
}
