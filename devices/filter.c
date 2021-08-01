//
// Created by Lao·Zhu on 2021/1/20.
//

#include "filter.h"
#include "string.h"

#define PI 3.1415926f

FirstOrder_Filter_t I_CapacitorF, I_ChassisF, V_CapacitorF, V_BaterryF, V_ChassisF, P_CapacitorF, P_ChassisF;

void Calculate_Coefficient(FirstOrder_Filter_t *param) {
    float RC = 1.0f / (2.0f * PI * param->CutOffFreq);
    param->Coeffecient1 = 1.0f / (1.0f + RC * param->SampleFreq);
    param->Coeffecient2 = (RC * param->SampleFreq) / (1.0f + RC * param->SampleFreq);
    param->Current_Result = 0;
    param->Last_Result = 0;
}

void Filter_Config(void) {
    I_CapacitorF.CutOffFreq = 200.0f;
    I_CapacitorF.SampleFreq = 10000.0f;
    Calculate_Coefficient(&I_CapacitorF);

    I_ChassisF.CutOffFreq = 5.0f;
    I_ChassisF.SampleFreq = 10000.0f;
    Calculate_Coefficient(&I_ChassisF);

    V_CapacitorF.CutOffFreq = 200.f;
    V_CapacitorF.SampleFreq = 10000.0f;
    Calculate_Coefficient(&V_CapacitorF);

    V_BaterryF.CutOffFreq = 200.0f;
    V_BaterryF.SampleFreq = 10000.0f;
    Calculate_Coefficient(&V_BaterryF);

    V_ChassisF.CutOffFreq = 5.0f;
    V_ChassisF.SampleFreq = 10000.0f;
    Calculate_Coefficient(&V_ChassisF);

    P_CapacitorF.CutOffFreq = 200.0f;
    P_CapacitorF.SampleFreq = 10000.0f;
    Calculate_Coefficient(&P_CapacitorF);

    P_ChassisF.CutOffFreq = 5.0f;
    P_ChassisF.SampleFreq = 10000.0f;
    Calculate_Coefficient(&P_ChassisF);
}

// 10khz
float FirstOrder_Filter_Calculate(FirstOrder_Filter_t *param) {
    param->Current_Result =
        param->Coeffecient1 * param->Current_Value +
            param->Coeffecient2 * param->Last_Result;
    param->Last_Result = param->Current_Result;
    return param->Current_Result;
}
