//
// Created by Lao·Zhu on 2021/4/5.
//

#include "protect.h"
#include "fsm.h"
#include "calculate.h"
#include "system.h"
#include "datasave.h"
#include "FreeRTOS.h"
#include "semphr.h"

unsigned short referee_avaiflag = 1, referee_time_counter = 0;
extern SemaphoreHandle_t Calibrate_Semaphore;

void Protect_Task(void *pvParameters) {
    Delayms(10);
    xSemaphoreTake(Calibrate_Semaphore, 0xFFFFFFFFUL);
    xSemaphoreGive(Calibrate_Semaphore);
    while (1) {
        referee_time_counter++;
        if (V_Baterry <= 20.0f && FSM_Status.FSM_Mode != Halt_Mode) {
            Delayms(100);
            if (V_Baterry <= 20.0f)
                FSM_Status.FSM_Mode = Halt_Mode;
        } else if (FSM_Status.FSM_Mode == Halt_Mode && V_Baterry > 20.0f) {
            DataSave_To_Flash(RePowerOn_Reset);
            Delayms(500);
            SoftReset();
        }
        if (referee_time_counter >= 1000) {
            referee_avaiflag = 0;
            referee_time_counter = 0;
        }
        Delayms(1);
    }
}
