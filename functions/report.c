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
#include "referee.h"

QueueHandle_t Referee_Data = NULL;

void Packup_Info(void) {
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

void Upload_Referee(void *pvParameters) {
    unsigned char Referee_Buf[128];
    Referee_Data = xQueueCreate(4, 128);
    while (1) {
        if (xQueueReceive(Referee_Data, Referee_Buf, 10) == pdTRUE) {
            Packup_Info();
            DTP_Transmit(&Upload_Package);
            for (unsigned char counter = 0; counter < 128; counter++) {
                HAL_UART_Transmit(&huart1, &Referee_Buf[counter], 1, 0xFFFFFFFFUL);
                Referee_unpack(Referee_Buf[counter]);
            }
        }
    }
}
