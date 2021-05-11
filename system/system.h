//
// Created by Lao·Zhu on 2021/1/20.
//

#ifndef _SYSTEM_SYSTEM_H_
#define _SYSTEM_SYSTEM_H_

typedef enum {
    Power_On_Reset = 1,
    RST_Pin_Reset = 2,
    Software_Reset = 3,
    IWDG_Reset = 4,
    WWDG_Reset = 5,
    LowPower_Reset = 6,
    Other_Reason = 7,
} Reset_Reason_e;

extern unsigned char FreeRTOS_Running_Flag;
void SystemClock_Config(void);
void MPU_Config(void);
void Cache_Config(void);
unsigned short FloatToInt16(float data0);
float Int16ToFloat(unsigned short data0);
void Delayus(unsigned int xus);
void Delayms(unsigned int xms);
void SoftReset(void);
Reset_Reason_e Check_ResetReason(void);

#endif //PROGRAM_SYSTEM_SYSTEM_H_
