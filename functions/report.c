//
// Created by Lao·Zhu on 2021/2/4.
//

#include "report.h"
#include "encrypt.h"
#include "system.h"
#include "calculate.h"
#include "pid.h"
#include "string.h"
#include "FreeRTOS.h"
#include "adc.h"
#include "filter.h"

#define ADC_COEFFICIENT 3.3f / 4096.0f

void Report_Task(void *pvParameters) {
    while (1) {
        DTP_Package_t pkg = {0};
        float ADC_Resultf[4];
        pkg.PID = 0;
        ADC_Resultf[0] = I_Capacitor;
        ADC_Resultf[1] = V_Capacitor;
        ADC_Resultf[2] = P_Capacitor;
        ADC_Resultf[3] = I_Chassis;
        for (unsigned char lcounter = 0; lcounter < 4; lcounter++) {
            pkg.Data[lcounter * 2] = FloatToInt16(ADC_Resultf[lcounter]) >> 8UL;
            pkg.Data[lcounter * 2 + 1] = FloatToInt16(ADC_Resultf[lcounter]) & 0x00ffUL;
        }
        DTP_Transmit(&pkg);
        Delayms(1);
        pkg.PID = 1;
        memset(ADC_Resultf, 0x00, sizeof(ADC_Resultf));
        ADC_Resultf[1] = (xPortGetFreeHeapSize() / 1024.0f);
        for (unsigned char lcounter = 0; lcounter < 4; lcounter++) {
            pkg.Data[lcounter * 2] = FloatToInt16(ADC_Resultf[lcounter]) >> 8UL;
            pkg.Data[lcounter * 2 + 1] = FloatToInt16(ADC_Resultf[lcounter]) & 0x00ffUL;
        }
        DTP_Transmit(&pkg);
        Delayms(40);
    }
}