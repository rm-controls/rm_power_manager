//
// Created by Lao·Zhu on 2021/5/3.
//

#ifndef PROGRAM_DEVICES_FLASH_H_
#define PROGRAM_DEVICES_FLASH_H_

#define W25X_WriteEnable        0x06
#define W25X_WriteDisable        0x04
#define W25X_ReadStatusReg1        0x05
#define W25X_ReadStatusReg2        0x35
#define W25X_ReadStatusReg3        0x15
#define W25X_WriteStatusReg1    0x01
#define W25X_WriteStatusReg2    0x31
#define W25X_WriteStatusReg3    0x11
#define W25X_ReadData            0x03
#define W25X_FastReadData        0x0B
#define W25X_FastReadDual        0x3B
#define W25X_PageProgram        0x02
#define W25X_BlockErase            0xD8
#define W25X_SectorErase        0x20
#define W25X_ChipErase            0xC7
#define W25X_PowerDown            0xB9
#define W25X_ReleasePowerDown    0xAB
#define W25X_DeviceID            0xAB
#define W25X_ManufactDeviceID    0x90
#define W25X_JedecDeviceID        0x9F
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

void w25qxx_config(void);
unsigned short w25qxx_get_total_sector(void);
void w25qxx_read(unsigned char *pBuffer, unsigned int ReadAddr, unsigned short NumByteToRead);
void w25qxx_write(unsigned char *pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite);
void w25qxx_erase_chip(void);

#endif //PROGRAM_DEVICES_FLASH_H_
