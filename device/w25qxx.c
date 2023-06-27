//
// Created by Lao·Zhu on 2021/5/3.
//

#include "w25qxx.h"
#include "system.h"
#include "spi.h"
#include "gpio.h"

#define FLASH_DELAY()   __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP()
#define W25QXX_CS_LOW   HAL_GPIO_WritePin(SPI_FLASH_CS_Port, SPI_FLASH_CS_Pin, GPIO_PIN_RESET)
#define W25QXX_CS_HIGH  HAL_GPIO_WritePin(SPI_FLASH_CS_Port, SPI_FLASH_CS_Pin, GPIO_PIN_SET)

unsigned short w25qxx_id = 0x0000;
static unsigned char w25qxx_sector_buffer[4096];

static unsigned char spi_readwrite_byte(unsigned char data) {
    unsigned char receive_data = 0;
    HAL_SPI_TransmitReceive(&hspi1, &data, &receive_data, 1, 0xFFFFFFFFUL);
    return receive_data;
}

static unsigned char w25qxx_read_sr1() {
    unsigned char byte;
    W25QXX_CS_LOW;
    FLASH_DELAY();
    spi_readwrite_byte(W25X_ReadStatusReg1);
    byte = spi_readwrite_byte(0Xff);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    return byte;
}

static void w25qxx_write_enable(void) {
    W25QXX_CS_LOW;
    FLASH_DELAY();
    spi_readwrite_byte(W25X_WriteEnable);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
}

static void w25qxx_wait_busy(void) {
    while ((w25qxx_read_sr1() & 0x01));
}

static unsigned short w25qxx_read_id(void) {
    unsigned short Temp = 0;
    W25QXX_CS_LOW;
    FLASH_DELAY();
    spi_readwrite_byte(0x90);
    spi_readwrite_byte(0x00);
    spi_readwrite_byte(0x00);
    spi_readwrite_byte(0x00);
    Temp |= spi_readwrite_byte(0xFF) << 8;
    Temp |= spi_readwrite_byte(0xFF);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    return Temp;
}

static void w25qxx_wakeup(void) {
    W25QXX_CS_LOW;
    FLASH_DELAY();
    spi_readwrite_byte(W25X_ReleasePowerDown);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    delayus(3);
}

static void w25qxx_erase_sector(unsigned int Dst_Addr) {
    Dst_Addr *= 4096;
    w25qxx_write_enable();
    w25qxx_wait_busy();
    W25QXX_CS_LOW;
    FLASH_DELAY();
    spi_readwrite_byte(W25X_SectorErase);
    spi_readwrite_byte((unsigned char) ((Dst_Addr) >> 16));
    spi_readwrite_byte((unsigned char) ((Dst_Addr) >> 8));
    spi_readwrite_byte((unsigned char) Dst_Addr);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    w25qxx_wait_busy();
}

void w25qxx_config(void) {
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    w25qxx_wakeup();
    w25qxx_id = w25qxx_read_id();
}

void w25qxx_read(unsigned char *pBuffer, unsigned int ReadAddr, unsigned short NumByteToRead) {
    unsigned short i;
    w25qxx_wakeup();
    W25QXX_CS_LOW;
    FLASH_DELAY();
    spi_readwrite_byte(W25X_ReadData);
    spi_readwrite_byte((unsigned char) ((ReadAddr) >> 16));
    spi_readwrite_byte((unsigned char) ((ReadAddr) >> 8));
    spi_readwrite_byte((unsigned char) ReadAddr);
    for (i = 0; i < NumByteToRead; i++)
        pBuffer[i] = spi_readwrite_byte(0XFF);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
}

static void w25qxx_write_page(unsigned char *pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite) {
    unsigned short i;
    w25qxx_write_enable();
    W25QXX_CS_LOW;
    FLASH_DELAY();
    spi_readwrite_byte(W25X_PageProgram);
    spi_readwrite_byte((unsigned char) ((WriteAddr) >> 16));
    spi_readwrite_byte((unsigned char) ((WriteAddr) >> 8));
    spi_readwrite_byte((unsigned char) WriteAddr);
    for (i = 0; i < NumByteToWrite; i++)
        spi_readwrite_byte(pBuffer[i]);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    w25qxx_wait_busy();
}

static void w25qxx_write_nocheck(unsigned char *pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite) {
    unsigned short pageremain;
    pageremain = 256 - WriteAddr % 256;
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite;
    while (1) {
        w25qxx_write_page(pBuffer, WriteAddr, pageremain);
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

void w25qxx_write(unsigned char *pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite) {
    unsigned int secpos;
    unsigned short secoff;
    unsigned short secremain;
    unsigned short i;
    unsigned char *W25QXX_BUF;
    for (unsigned short counter = 0; counter < 4096; ++counter)
        w25qxx_sector_buffer[counter] = 0x00;
    W25QXX_BUF = w25qxx_sector_buffer;
    secpos = WriteAddr / 4096;
    secoff = WriteAddr % 4096;
    secremain = 4096 - secoff;
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite;
    while (1) {
        w25qxx_read(W25QXX_BUF, secpos * 4096, 4096);
        for (i = 0; i < secremain; i++)
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break;
        if (i < secremain) {
            w25qxx_erase_sector(secpos);
            for (i = 0; i < secremain; i++)
                W25QXX_BUF[i + secoff] = pBuffer[i];
            w25qxx_write_nocheck(W25QXX_BUF, secpos * 4096, 4096);

        } else
            w25qxx_write_nocheck(pBuffer, WriteAddr, secremain);
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

void w25qxx_erase_chip(void) {
    w25qxx_write_enable();
    w25qxx_wait_busy();
    W25QXX_CS_LOW;
    FLASH_DELAY();
    spi_readwrite_byte(W25X_ChipErase);
    FLASH_DELAY();
    W25QXX_CS_HIGH;
    w25qxx_wait_busy();
}
