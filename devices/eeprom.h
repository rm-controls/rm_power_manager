//
// Created by Lao·Zhu on 2021/4/29.
//

#ifndef PROGRAM_DEVICES_EEPROM_H_
#define PROGRAM_DEVICES_EEPROM_H_

unsigned char EEPROM_ReadOneByte(unsigned char ReadAddr);
void EEPROM_WriteOneByte(unsigned char WriteAddr, unsigned char DataToWrite);
void EEPROM_Write(unsigned char WriteAddr, unsigned char *pBuffer, unsigned char NumToWrite);
void EEPROM_Read(unsigned char ReadAddr, unsigned char *pBuffer, unsigned char NumToRead);

#endif //PROGRAM_DEVICES_EEPROM_H_
