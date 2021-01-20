//
// Created by Lao·Zhu on 2021/1/16.
//

#include "pid.h"

#define Ufabs(x) x >= 0 ? x : -1 * x

float PID_Get_Result(PID_Structure *PID_Handler) {
    float Error = (PID_Handler->User - PID_Handler->Collect[0]);
    PID_Handler->I_Sum =
        PID_Handler->I_Sum + (float) 0.5 * (float) ((PID_Handler->User - PID_Handler->Collect[1]) + Error);
    if (Ufabs(PID_Handler->I_Sum) > PID_Handler->I_Sum_Max && PID_Handler->I_Sum > 0)
        PID_Handler->I_Sum = PID_Handler->I_Sum_Max;
    else if (Ufabs(PID_Handler->I_Sum) > PID_Handler->I_Sum_Max && PID_Handler->I_Sum < 0)
        PID_Handler->I_Sum = (-1.0f * PID_Handler->I_Sum_Max);
    PID_Handler->Result = (PID_Handler->Kp) * Error
        + (PID_Handler->Ki * PID_Handler->I_Sum)
        + (PID_Handler->Kd * ((PID_Handler->User - PID_Handler->Collect[1]) - Error))
        + (PID_Handler->Offset);
    if (PID_Handler->Result > PID_Handler->Maxinum)
        PID_Handler->Result = PID_Handler->Maxinum;
    if (PID_Handler->Result < PID_Handler->Minium)
        PID_Handler->Result = PID_Handler->Minium;
    return PID_Handler->Result;
}
