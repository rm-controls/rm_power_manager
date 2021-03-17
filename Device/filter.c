//
// Created by Lao·Zhu on 2021/1/20.
//

#include "filter.h"
#include "string.h"

FirstOrder_Filter_t I_CapacitorF, I_ChassisF, V_CapacitorF, V_BaterryF, V_ChassisF, P_CapacitorF, P_ChassisF;

void Filter_Config(void) {
    memset((void *) &I_CapacitorF, 0x00, sizeof(FirstOrder_Filter_t));
    memset((void *) &I_ChassisF, 0x00, sizeof(FirstOrder_Filter_t));
    memset((void *) &V_CapacitorF, 0x00, sizeof(FirstOrder_Filter_t));
    memset((void *) &V_BaterryF, 0x00, sizeof(FirstOrder_Filter_t));
    memset((void *) &V_ChassisF, 0x00, sizeof(FirstOrder_Filter_t));
    memset((void *) &P_CapacitorF, 0x00, sizeof(FirstOrder_Filter_t));
    memset((void *) &P_ChassisF, 0x00, sizeof(FirstOrder_Filter_t));

    I_CapacitorF.CufOff_Freq = 20.0f;
    I_ChassisF.CufOff_Freq = 20.0f;
    V_CapacitorF.CufOff_Freq = 20.f;
    V_BaterryF.CufOff_Freq = 20.0f;
    V_ChassisF.CufOff_Freq = 20.0f;
    P_CapacitorF.CufOff_Freq = 20.0f;
    P_ChassisF.CufOff_Freq = 20.0f;

    I_CapacitorF.Coeffecient = 6.283185f * I_CapacitorF.CufOff_Freq * 0.001f;
    I_ChassisF.Coeffecient = 6.283185f * I_ChassisF.CufOff_Freq * 0.001f;
    V_CapacitorF.Coeffecient = 6.283185f * V_CapacitorF.CufOff_Freq * 0.001f;
    V_BaterryF.Coeffecient = 6.283185f * V_BaterryF.CufOff_Freq * 0.001f;
    V_ChassisF.Coeffecient = 6.283185f * V_ChassisF.CufOff_Freq * 0.001f;
    P_CapacitorF.Coeffecient = 6.283185f * P_CapacitorF.CufOff_Freq * 0.001f;
    P_ChassisF.Coeffecient = 6.283185f * P_ChassisF.CufOff_Freq * 0.001f;
}

float FirstOrder_Filter_Calculate(FirstOrder_Filter_t *param) {
    param->Current_Result =
        param->Coeffecient * param->Current_Value + (1.0f - param->Coeffecient) * param->Last_Result;
    param->Last_Result = param->Current_Result;
    return param->Current_Result;
}
