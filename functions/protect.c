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

extern SemaphoreHandle_t Calibrate_Semaphore;

void Protect_Task(void *pvParameters) {
    FSM_Status_t Record_FSM;
    Delayms(10);
    xSemaphoreTake(Calibrate_Semaphore, 0xFFFFFFFFUL);
    xSemaphoreGive(Calibrate_Semaphore);
    Record_FSM.Charge_Mode = FSM_Status.uCharge_Mode;
    while (1) {
        if (V_Capacitor >= 15.5f) { // OverVoltage Protection
            Record_FSM.Charge_Mode = FSM_Status.Charge_Mode;
            FSM_Status.Charge_Mode = Zero_Power_Charge;
        } else if (V_Capacitor <= 15.0f) {
            if (FSM_Status.Charge_Mode == Zero_Power_Charge)
                FSM_Status.Charge_Mode = Record_FSM.Charge_Mode;
        }
        if (V_Baterry <= 20.0f) {
            Delayms(100);
            if (V_Baterry <= 20.0f)
                FSM_Status.FSM_Mode = Halt_Mode;
        } else if (FSM_Status.FSM_Mode == Halt_Mode) {
            DataSave_To_Flash(RePowerOn_Reset);
            Delayms(1000);
            SoftReset();
        }
        Delayms(1);
    }
}
