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
//    Delayms(1000);
    while (1) {
        if (V_Capacitor >= 15.0f) {     //Enter Over Power Mode
            FSM_Status.FSM_Mode = OverPower_Mode;
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        } else if (V_Capacitor <= 8.0f && V_Capacitor > 7.0f &&
            FSM_Status.FSM_Mode == OverPower_Mode) {
            FSM_Status.FSM_Mode = Transition_Mode;
        } else if (V_Capacitor <= 7.0f) {   //Exit OverPower Mode
            FSM_Status.FSM_Mode = Normal_Mode;
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        }
        Delayms(1);
    }
}
