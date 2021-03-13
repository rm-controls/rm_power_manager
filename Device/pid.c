//
// Created by Lao·Zhu on 2021/1/16.
//

#include "pid.h"
#include "system.h"
#include "dac.h"
#include "calculate.h"
#include "port.h"
#include "decrypt.h"

PID_Structure PID_Capacitor_P, PID_Capacitor_C;

#define Ufabs(x) x >= 0 ? x : -1 * x

void PID_CalculateTask(void *pvParameters) {
    while (1) {
//        if (PID_Capacitor.User >= 10) {
//        PID_Capacitor_P.Collect[1] = PID_Capacitor_P.Collect[0];
//        PID_Capacitor_P.Collect[0] = P_Capacitor;
//        PID_Get_Result(&PID_Capacitor_P);

//        PID_Capacitor_C.User = PID_Capacitor_P.Result;
//        PID_Capacitor_C.Collect[1] = PID_Capacitor_C.Collect[0];
//        PID_Capacitor_C.Collect[0] = I_Capacitor;
//        PID_Get_Result(&PID_Capacitor_C);
//        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
//        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, PID_Capacitor_C.Result / 1);
//        } else
//            HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
//        if (V_Capacitor >= 15.5f) {
//            HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
//            while (1) {
//                Delayms(1);
//                if (V_Capacitor <= 15.0f)
//                    break;
//            }
//            HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
//        }
        Delayms(2);
    }
}

void PID_ValueConfig(void) {
    PID_Capacitor_P.Kp = 35.81f;
    PID_Capacitor_P.Ki = 1.33f;
    PID_Capacitor_P.Kd = -12.0f;
    PID_Capacitor_P.User = 0;
    PID_Capacitor_P.I_Sum_Max = 300.0f;
    PID_Capacitor_P.Maxinum = 5.0f;
    PID_Capacitor_P.Minium = 0;
    PID_Capacitor_P.Offset = 0;

    PID_Capacitor_C.Kp = 666.6f;
    PID_Capacitor_C.Ki = 19.73f;
    PID_Capacitor_C.Kd = -20.83f;
    PID_Capacitor_C.User = 0;
    PID_Capacitor_C.I_Sum_Max = 19.58f;
    PID_Capacitor_C.Maxinum = 4095;
    PID_Capacitor_C.Minium = 1;
    PID_Capacitor_C.Offset = 0;
}

void PID_Get_Result(PID_Structure *PID_Handler) {
    float Error = (PID_Handler->User - PID_Handler->Collect[0]);
    PID_Handler->I_Sum =
        PID_Handler->I_Sum + (float) 0.5 * (float) ((PID_Handler->User - PID_Handler->Collect[1]) + Error);
    if (Ufabs(PID_Handler->I_Sum) > PID_Handler->I_Sum_Max && PID_Handler->I_Sum > 0)
        PID_Handler->I_Sum = PID_Handler->I_Sum_Max;
    else if (Ufabs(PID_Handler->I_Sum) > PID_Handler->I_Sum_Max && PID_Handler->I_Sum < 0)
        PID_Handler->I_Sum = (-1.0f * PID_Handler->I_Sum_Max);
    PID_Handler->Result = (PID_Handler->Kp) * Error
        + (PID_Handler->Ki * PID_Handler->I_Sum)
        + (PID_Handler->Kd * (Error - (PID_Handler->User - PID_Handler->Collect[1])))
        + (PID_Handler->Offset);
    if (PID_Handler->Result > PID_Handler->Maxinum)
        PID_Handler->Result = PID_Handler->Maxinum;
    if (PID_Handler->Result < PID_Handler->Minium)
        PID_Handler->Result = PID_Handler->Minium;
}
