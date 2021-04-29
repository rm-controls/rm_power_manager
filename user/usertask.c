//
// Created by Lao·Zhu on 2021/2/4.
//

#include "main.h"

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
                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
            } else if (V_Capacitor <= 8.0f && V_Capacitor > 7.0f &&
                FSM_Status.FSM_Mode == OverPower_Mode) {
                FSM_Status.FSM_Mode = Transition_Mode;
            } else if (V_Capacitor <= 7.0f) {   //Exit OverPower Mode
                FSM_Status.FSM_Mode = Normal_Mode;
                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
            }
        }
        Delayms(1);
    }
}

void LCD_Refresh(void *pvParameters) {
    GUI_Init();
    GUI_DrawCircle(50, 50, 40, C_YELLOW_GREEN, Filled);
    GUI_DrawCircle(50, 50, 40, C_DARK_RED, UnFilled);
    GUI_DrawLine(0, 0, 20, 40, C_BROWN);
    GUI_DrawRectangle(100, 100, 20, 40, C_DARK_OLIVE_GREEN, Filled);
    GUI_DrawRectangle(100, 100, 20, 40, C_DARK_RED, UnFilled);
    GUI_DrawRoundRectangle(20, 100, 70, 50, 10, C_BLUE, Filled);
    GUI_DrawRoundRectangle(20, 100, 70, 50, 10, C_DARK_RED, UnFilled);
    while (1) {
        Delayms(200);
    }
}
