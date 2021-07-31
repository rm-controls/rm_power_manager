//
// Created by Lao·Zhu on 2021/2/4.
//

#include "main.h"

extern SemaphoreHandle_t Calibrate_Semaphore;
unsigned char complex_calibrate_flag = 0;

void UserTask(void *pvParameters) {
    FSM_Status.FSM_Mode = NoCharge_Mode;
    FSM_Status.uExpect_Mode = Proportional_Expect;
    FSM_Status.uCharge_Mode = Proportional_Charge;
    FSM_Status.P_Charge = 0.15f;
    FSM_Status.P_Expect = 0.85f;
    FSM_Status.Max_Power = 200;
    while (1) {
        if (FSM_Status.FSM_Mode != Halt_Mode) {
            if (referee_data_.game_status_.game_progress == 1 && complex_calibrate_flag == 0) {
                FSM_Status.FSM_Mode = OverPower_Mode;
                while (referee_data_.game_status_.game_progress == 1
                    && V_Capacitor > 12.0f)        // wait until capacitor voltage is above 12V
                    Delayms(1);
                FSM_Status.FSM_Mode = NoCharge_Mode;
                xSemaphoreTake(Calibrate_Semaphore, 0xFFFFFFFFUL);
                GUI_Clear(C_WHITE);
                GUI_Printf(22, 74, C_DARK_GREEN, C_WHITE, "Calibrating...");
                ComplexPower_Calibrate();
                xSemaphoreGive(Calibrate_Semaphore);
                complex_calibrate_flag = 1;
                FSM_Status.FSM_Mode = OverPower_Mode;
                while (referee_data_.game_status_.game_progress == 1 && Capacitor_Percent < 1.0f)
                    Delayms(1);
            } else {
                switch (Setting_OptiSchemes) {
                    case Normal_Optimized:FSM_Status.FSM_Mode = NoCharge_Mode;
                        break;
                    case ChargeFirst_Optimized:
                        if (Capacitor_Percent < 0.95f)
                            FSM_Status.FSM_Mode = Normal_Mode;
                        else if (Capacitor_Percent > 1.0f)
                            FSM_Status.FSM_Mode = NoCharge_Mode;
                        break;
                    case UseFirst_Optimized:
                        if (Capacitor_Percent < 0.05f)
                            FSM_Status.FSM_Mode = NoCharge_Mode;
                        else
                            FSM_Status.FSM_Mode = OverPower_Mode;
                        break;
                    case SucapTest_Optimized:FSM_Status.FSM_Mode = SucapTest_Mode;
                        while (Capacitor_Percent < 0.95f && Setting_OptiSchemes == SucapTest_Optimized)
                            Delayms(1);
                        FSM_Status.FSM_Mode = Halt_Mode;
                        while (Setting_OptiSchemes == SucapTest_Optimized)
                            Delayms(1);
                        FSM_Status.FSM_Mode = NoCharge_Mode;
                        break;
                    default: FSM_Status.FSM_Mode = Normal_Mode;
                        break;
                }
            }
        }
        Delayms(1);
    }
}

void LCD_Refresh(void *pvParameters) {
    unsigned char running_timer_counter = 0;
    GUI_Printf(22, 74, C_DARK_GREEN, C_WHITE, "Calibrating...");
    xSemaphoreTake(Calibrate_Semaphore, 0xFFFFFFFFUL);
    xSemaphoreGive(Calibrate_Semaphore);
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
    while (1) {
        xSemaphoreTake(Calibrate_Semaphore, 0xFFFFFFFFUL);
        xSemaphoreGive(Calibrate_Semaphore);
        Form_UpdateEvent();
        running_timer_counter++;
        if (running_timer_counter == 20)
            GUI_DrawCircle(123, 5, 4, C_WHITE, Filled);
        else if (running_timer_counter == 40) {
            running_timer_counter = 0;
            GUI_DrawCircle(123, 5, 4, C_DARK_GREEN, Filled);
        }
        Delayms(50);
    }
}
