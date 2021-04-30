//
// Created by Lao·Zhu on 2021/2/4.
//

#include "main.h"
extern SemaphoreHandle_t Calibrate_Semaphore;
void UserTask(void *pvParameters) {
    FSM_Status.uExpect_Mode = Remain_Power_Expect;
    FSM_Status.uCharge_Mode = Const_Power_Charge;
    FSM_Status.FSM_Mode = Normal_Mode;
    FSM_Status.Charge_Power = 20;
    FSM_Status.Max_Power = 100;
    while (1) {
        if (FSM_Status.FSM_Mode != Halt_Mode) {
            if (V_Capacitor >= 15.0f) {         //Enter Over Power Mode
                FSM_Status.FSM_Mode = OverPower_Mode;
            } else if (V_Capacitor <= 8.0f && V_Capacitor > 7.0f &&
                FSM_Status.FSM_Mode == OverPower_Mode) {
                FSM_Status.FSM_Mode = Transition_Mode;
            } else if (V_Capacitor <= 7.0f) {   //Exit OverPower Mode
                FSM_Status.FSM_Mode = Normal_Mode;
            }
        }
        Delayms(1);
    }
}

void LCD_Refresh(void *pvParameters) {
    GUI_Printf(22, 74, C_DARK_BLUE, C_WHITE, "Calibrating...");
    xSemaphoreTake(Calibrate_Semaphore, 0xFFFFFFFFUL);
    xSemaphoreGive(Calibrate_Semaphore);
    MainWidget_Init();
    while (1) {
        MainWidget_Update();
        GUI_DrawCircle(123, 5, 4, C_WHITE, Filled);
        Delayms(1000);
        MainWidget_Update();
        GUI_DrawCircle(123, 5, 4, C_DARK_GREEN, Filled);
        Delayms(1000);
    }
}
