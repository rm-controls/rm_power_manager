//
// Created by Lao·Zhu on 2021/1/20.
//

#ifndef _SYSTEM_SYSTEM_H_
#define _SYSTEM_SYSTEM_H_

void SystemClock_Config(void);
void MPU_Config(void);
void Cache_Config(void);
unsigned short FloatToInt16(float data0);
float Int16ToFloat(unsigned short data0);
void Delayus(unsigned int xus);
void Delayms(unsigned int xms);
void SoftReset(void);

#endif //PROGRAM_SYSTEM_SYSTEM_H_
