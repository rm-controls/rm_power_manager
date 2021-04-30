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
//                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
            } else if (V_Capacitor <= 8.0f && V_Capacitor > 7.0f &&
                FSM_Status.FSM_Mode == OverPower_Mode) {
                FSM_Status.FSM_Mode = Transition_Mode;
            } else if (V_Capacitor <= 7.0f) {   //Exit OverPower Mode
                FSM_Status.FSM_Mode = Normal_Mode;
//                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
            }
        }
        Delayms(1);
    }
}

void LCD_Refresh(void *pvParameters) {
    GUI_Init();
    MainWidget_Init();
//    unsigned char address = 0;
//    for (address = 0; address < 0xff; address++) {
//        IIC_Start();
//        if (IIC_SendByte(address) == 0)
//            break;
//        IIC_Stop();
//        Delayms(1);
//    }
//    GUI_Printf(0, 0, C_BLACK, C_WHITE, "0x%x", address);
    while (1) {
//        GUI_CurveAppend(&VChassis_Curve, V_Chassis);
//        GUI_CurveAppend(&VCapacitor_Curve, V_Capacitor);
        Delayms(100);
    }
}
