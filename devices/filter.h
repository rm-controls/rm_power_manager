//
// Created by Lao·Zhu on 2021/1/20.
//

#ifndef _DEVICE_FILTER_H_
#define _DEVICE_FILTER_H_

typedef struct {
    float Coeffecient;
    float CufOff_Freq;
    float Last_Result;
    float Current_Value;
    float Current_Result;
} FirstOrder_Filter_t;

extern FirstOrder_Filter_t I_CapacitorF, I_ChassisF, V_CapacitorF, V_BaterryF, V_ChassisF, P_CapacitorF, P_ChassisF;
void Filter_Config(void);
float FirstOrder_Filter_Calculate(FirstOrder_Filter_t *param);

#endif //PROGRAM_DEVICE_FILTER_H_
