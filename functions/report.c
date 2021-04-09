//
// Created by Lao·Zhu on 2021/2/4.
//

#include "report.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "usart.h"

#if DEBUG_PARAM != 1
#include "refree.h"
#include "stm32h7xx_hal.h"
QueueHandle_t Refree_Data = NULL;
#else
#include "encrypt.h"
#include "system.h"
#include "calculate.h"
#include "pid.h"
#include "decrypt.h"
QueueHandle_t DTP_Data = NULL;
TaskHandle_t ReportTask_Handler;
#endif

void Upload_Refree(void *pvParameters) {
#if DEBUG_PARAM != 1
    unsigned char Refree_Buf[64];
    Refree_Data = xQueueCreate(4, 64);
    while (1) {
        if (xQueueReceive(Refree_Data, Refree_Buf, 10) == pdTRUE) {
            for (unsigned char counter = 0; counter < 64; counter++) {
                HAL_UART_Transmit(&huart1, &Refree_Buf[counter], 1, 0xFFFFFFFFUL);
                Referee_unpack(Refree_Buf[counter]);
            }
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
#if DEBUG_PARAM == 1
void Report_Task(void *pvParameters) {
    while (1) {
        DTP_Package_t pkg = {0};
        float ADC_Resultf[4];
        pkg.PID = 0;
        ADC_Resultf[0] = P_Capacitor;
        ADC_Resultf[1] = P_Chassis;
        ADC_Resultf[2] = V_Chassis;
        ADC_Resultf[3] = I_Chassis;
        for (unsigned char lcounter = 0; lcounter < 4; lcounter++) {
            pkg.Data[lcounter * 2] = FloatToInt16(ADC_Resultf[lcounter]) >> 8UL;
            pkg.Data[lcounter * 2 + 1] = FloatToInt16(ADC_Resultf[lcounter]) & 0x00ffUL;
        }
        DTP_Transmit(&pkg);
        Delayms(40);
    }
}
#endif