//
// Created by Lao·Zhu on 2021/2/4.
//

#include "report.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "usart.h"
#include "calculate.h"
#include "system.h"
#include "encrypt.h"
#include "refree.h"

QueueHandle_t Refree_Data = NULL;

void Packup_Info(void) {
    W_Capacitor = 0.5f * 15 * V_Capacitor * V_Capacitor - 367.5f;
    if (W_Capacitor < 0)
        W_Capacitor = 0;
    float Capacitor_Percent = W_Capacitor / 1434.375f;
    Upload_Package.PID = 0;
    Upload_Package.Data[0] = FloatToInt16(P_Chassis) >> 8UL;
    Upload_Package.Data[1] = FloatToInt16(P_Chassis) & 0x00ffUL;
    Upload_Package.Data[2] = FloatToInt16(EP_Chassis) >> 8UL;
    Upload_Package.Data[3] = FloatToInt16(EP_Chassis) & 0x00ffUL;
    Upload_Package.Data[4] = FloatToInt16(V_Chassis) >> 8UL;
    Upload_Package.Data[5] = FloatToInt16(V_Chassis) & 0x00ffUL;
    Upload_Package.Data[6] = FloatToInt16(Capacitor_Percent) >> 8UL;
    Upload_Package.Data[7] = FloatToInt16(Capacitor_Percent) & 0x00ffUL;
}

void Upload_Refree(void *pvParameters) {
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
}
