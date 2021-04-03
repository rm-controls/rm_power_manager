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
#include "pid.h"

#if DEBUG_PARAM != 1
QueueHandle_t Refree_Data = NULL, Forward_Data = NULL;
#else
QueueHandle_t DTP_Data = NULL;
TaskHandle_t ReportTask_Handler;
#endif

void Upload_Refree(void *pvParameters) {
#if DEBUG_PARAM != 1
    unsigned char Refree_Buf[64], DTP_Buf[16];
    Refree_Data = xQueueCreate(4, 64);
    Forward_Data = xQueueCreate(4, 16);
    while (1) {
        if (xQueueReceive(Refree_Data, Refree_Buf, 10) == pdTRUE) {
            for (unsigned char counter = 0; counter < 64; counter++)
                Referee_unpack(Refree_Buf[counter]);
            HAL_UART_Transmit(&huart1, Refree_Buf, 64, 0xFFFFFFFFUL);
        }
        if (xQueueReceive(Forward_Data, DTP_Buf, 10) == pdTRUE) {
            HAL_UART_Transmit(&huart2, DTP_Buf, 16, 0xFFFFFFFFUL);
        }
    }
#else
    unsigned char DTP_Buf[10];
    DTP_Data = xQueueCreate(8, 10);
    xTaskCreate(Report_Task, "ReportTask", 512, NULL, 1, &ReportTask_Handler);
    while (1) {
        if (xQueueReceive(DTP_Data, DTP_Buf, 10) == pdTRUE)
            for (unsigned char counter = 0; counter < 10; counter++)
                DTP_Received_CallBack(DTP_Buf[counter]);
    }
#endif
}

void Report_Task(void *pvParameters) {
    while (1) {
        DTP_Package_t pkg = {0};
        float ADC_Resultf[4];
        pkg.PID = 0;
        ADC_Resultf[0] = P_Capacitor;
        ADC_Resultf[1] = PID_Capacitor.User;
        ADC_Resultf[2] = V_Capacitor;
        ADC_Resultf[3] = I_Capacitor;
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