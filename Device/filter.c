//
// Created by Lao·Zhu on 2021/1/20.
//

#include "filter.h"
#include "string.h"

FirstOrder_Filter_t I_CapacitorF, I_ChassisF, V_CapacitorF, V_BaterryF, V_ChassisF, P_CapacitorF;

void FirstOrder_Filter_Config(void) {
    memset((void *) &I_CapacitorF, 0x00, sizeof(I_CapacitorF));
    memset((void *) &I_ChassisF, 0x00, sizeof(I_ChassisF));
    memset((void *) &V_CapacitorF, 0x00, sizeof(V_CapacitorF));
    memset((void *) &V_BaterryF, 0x00, sizeof(V_BaterryF));
    memset((void *) &V_ChassisF, 0x00, sizeof(V_ChassisF));
    memset((void *) &P_CapacitorF, 0x00, sizeof(P_CapacitorF));
    I_CapacitorF.CufOff_Freq = 0.3f;
    I_ChassisF.CufOff_Freq = 0.3f;
    V_CapacitorF.CufOff_Freq = 0.3f;
    V_BaterryF.CufOff_Freq = 0.3f;
    V_ChassisF.CufOff_Freq = 0.3f;
    P_CapacitorF.CufOff_Freq = 0.3f;
}

void FirstOrder_Filter_Calculate(FirstOrder_Filter_t *param) {
    float alpha = 6.283185f / (6.283185f + param->CufOff_Freq);
    param->Last_Result = param->Current_Result;
    param->Current_Result = (1.0f - alpha) * param->Current_Value + alpha * param->Last_Result;
}
