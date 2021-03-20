//
// Created by Lao·Zhu on 2021/2/4.
//

#include "calculate.h"
#include "adc.h"
#include "filter.h"
#include "dac.h"
#include "port.h"

#define ADC_COEFFICIENT 3.3f / 4096.0f

float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry, V_Chassis, P_Chassis, P_Capacitor;
unsigned short I_CapOffset, I_ChaOffset;

void Sensor_Config(void) {
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x01);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);
    I_CapOffset = ADC_FinalResult[0];
    I_ChaOffset = ADC_FinalResult[1];
    HAL_Delay(100);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
}

void Calculate_Power(void) {
    I_CapacitorF.Current_Value = (float) (ADC_FinalResult[0] - I_CapOffset) * ADC_COEFFICIENT * 10.0f;
    I_Capacitor = FirstOrder_Filter_Calculate(&I_CapacitorF);

    I_ChassisF.Current_Value = (float) (ADC_FinalResult[1] - I_ChaOffset) * ADC_COEFFICIENT * 10.0f;
    I_Chassis = FirstOrder_Filter_Calculate(&I_ChassisF);

    V_BaterryF.Current_Value = (float) ADC_FinalResult[2] * ADC_COEFFICIENT * 21.0f;
    V_Baterry = FirstOrder_Filter_Calculate(&V_BaterryF);

    V_CapacitorF.Current_Value = (float) ADC_FinalResult[3] * ADC_COEFFICIENT * 21.0f;
    V_Capacitor = FirstOrder_Filter_Calculate(&V_CapacitorF);

    V_ChassisF.Current_Value = (float) ADC_FinalResult[4] * ADC_COEFFICIENT * 21.0f;
    V_Chassis = FirstOrder_Filter_Calculate(&V_ChassisF);

    P_ChassisF.Current_Value = I_Chassis * V_Chassis;
    P_Chassis = FirstOrder_Filter_Calculate(&P_ChassisF);

    P_CapacitorF.Current_Value = V_Baterry * I_Capacitor;
    P_Capacitor = FirstOrder_Filter_Calculate(&P_CapacitorF);
}