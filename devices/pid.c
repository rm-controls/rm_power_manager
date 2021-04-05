//
// Created by Lao·Zhu on 2021/1/16.
//

#include "pid.h"
#include "system.h"
#include "dac.h"
#include "calculate.h"
#include "port.h"

PID_Structure PID_Capacitor;

void PID_CalculateTask(void *pvParameters) {
    while (1) {     // OverVoltage Protection
        Delayms(1);
        if (V_Capacitor <= 15.0f)
            break;
    }
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    while (1) {
        PID_Get_Result(&PID_Capacitor, P_Capacitor);
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (unsigned short) PID_Capacitor.Result);
        Delayms(1);
    }
}

void PID_ValueConfig(void) {
    PID_Capacitor.Kp = 1.0f;
    PID_Capacitor.Ki = 10.0f;
    PID_Capacitor.Kd = 7.5f;
    PID_Capacitor.User = 0;
    PID_Capacitor.I_Sum_Max = 185.0f;
    PID_Capacitor.Maxinum = 2730;       //2V
    PID_Capacitor.Minium = 1;
}

void PID_Get_Result(PID_Structure *PID_Handler, float collect) {
    float Error = PID_Handler->User - collect;
    PID_Handler->I_Sum = PID_Handler->I_Sum + Error;
    if (PID_Handler->I_Sum > PID_Handler->I_Sum_Max && PID_Handler->I_Sum > 0)
        PID_Handler->I_Sum = PID_Handler->I_Sum_Max;
    else if (PID_Handler->I_Sum < (-1.0 * PID_Handler->I_Sum_Max) && PID_Handler->I_Sum < 0)
        PID_Handler->I_Sum = (-1.0f * PID_Handler->I_Sum_Max);
    PID_Handler->Result = PID_Handler->Kp * Error
        + PID_Handler->Ki * PID_Handler->I_Sum
        + PID_Handler->Kd * (Error - PID_Handler->LastError);
    if (PID_Handler->Result > PID_Handler->Maxinum)
        PID_Handler->Result = PID_Handler->Maxinum;
    if (PID_Handler->Result < PID_Handler->Minium)
        PID_Handler->Result = PID_Handler->Minium;
    PID_Handler->LastError = Error;
}
