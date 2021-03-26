//
// Created by Lao·Zhu on 2021/2/4.
//

#include "report.h"
#include "encrypt.h"
#include "system.h"
#include "calculate.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "refree.h"
#include "decrypt.h"
#include "stm32h7xx_hal.h"
#include "usart.h"

#define ADC_COEFFICIENT 3.3f / 4096.0f
QueueHandle_t Refree_Data = NULL, DTP_Data = NULL;

void Upload_Refree(void *pvParameters) {
    DTP_Package_t pkg = {.PID=0, .Data={0, 1, 2, 3, 4, 5, 6, 7},};
    unsigned char Refree_Buf[64], DTP_Buf[16];
    Refree_Data = xQueueCreate(4, 64);
    DTP_Data = xQueueCreate(4, 16);
    while (1) {
        if (xQueueReceive(Refree_Data, Refree_Buf, 10) == pdTRUE) {
            for (unsigned char counter = 0; counter < 64; counter++)
                Referee_unpack(Refree_Buf[counter]);
            HAL_UART_Transmit(&huart1, Refree_Buf, 64, 0xFFFFFFFFUL);
        } else if (referee_unpack_obj.unpack_step == kStepHeaderSof)
            DTP_Transmit(&pkg);
        if (xQueueReceive(DTP_Data, DTP_Buf, 10) == pdTRUE) {
            for (unsigned char counter = 0; counter < 16; counter++)
                DTP_Received_CallBack(DTP_Buf[counter]);
        }
    }
}

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