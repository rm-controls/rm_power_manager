//
// Created by Lao·Zhu on 2021/2/4.
//

#include "calculate.h"
#include "adc.h"
#include "filter.h"
#include "dac.h"
#include "port.h"
#include "encrypt.h"
#include "system.h"
#include "refree.h"

#define ADC_COEFFICIENT 3.0f / 4096.0f

Power_Calibrate_t Capacitor_Calibrate, Chassis_Calibrate;
float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry, V_Chassis
, P_Chassis, P_Capacitor, EP_Chassis, W_Capacitor;
unsigned short I_CapOffset, I_ChaOffset;

void Calibrate_Power(void) {
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (unsigned short) (5460.0f / V_Capacitor)); //20W
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    Delayms(500);
    Capacitor_Calibrate.Pd[0] = P_Capacitor;
    Capacitor_Calibrate.Pr[0] = referee_data_.power_heat_data_.chassis_power;
    Capacitor_Calibrate.Iw[0] = I_Capacitor;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (unsigned short) (10920.0f / V_Capacitor)); //40W
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    Delayms(500);
    Capacitor_Calibrate.Pd[1] = P_Capacitor;
    Capacitor_Calibrate.Pr[1] = referee_data_.power_heat_data_.chassis_power;
    Capacitor_Calibrate.Iw[1] = I_Capacitor;
    Capacitor_Calibrate.Rw = (Capacitor_Calibrate.Pr[0] - Capacitor_Calibrate.Pd[0])
        / (Capacitor_Calibrate.Iw[0] * Capacitor_Calibrate.Iw[0]);
    Capacitor_Calibrate.Rw = (Capacitor_Calibrate.Rw + (Capacitor_Calibrate.Pr[1] - Capacitor_Calibrate.Pd[1])
        / (Capacitor_Calibrate.Iw[1] * Capacitor_Calibrate.Iw[1])) * 0.5f;
}

void Sensor_Config(void) {
    Capacitor_Calibrate.Rw = 0;
    Chassis_Calibrate.Rw = 0;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x00);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_Delay(500);
    I_CapOffset = ADC_FinalResult[0];
    I_ChaOffset = ADC_FinalResult[1];
    HAL_Delay(500);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
}

void Calculate_Power(void) {
    I_CapacitorF.Current_Value = (float) (ADC_FinalResult[0] - I_CapOffset) * ADC_COEFFICIENT * 20.0f;
    I_Capacitor = FirstOrder_Filter_Calculate(&I_CapacitorF);

    I_ChassisF.Current_Value = (float) (ADC_FinalResult[1] - I_ChaOffset) * ADC_COEFFICIENT * 20.0f;
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
    P_Capacitor = FirstOrder_Filter_Calculate(&P_CapacitorF) + I_Capacitor * I_Capacitor * Capacitor_Calibrate.Rw;

    W_Capacitor = 0.5f * 15 * V_Capacitor * V_Capacitor - 367.5f;
    if (W_Capacitor < 0)
        W_Capacitor = 0;
    float Capacitor_Percent = W_Capacitor / 1434.375f;

    Upload_Package.PID = 0;
    Upload_Package.Data[0] = FloatToInt16(P_Chassis) >> 8UL;
    Upload_Package.Data[1] = FloatToInt16(P_Chassis) & 0x00ffUL;
    Upload_Package.Data[2] = FloatToInt16(EP_Chassis) >> 8UL;
    Upload_Package.Data[3] = FloatToInt16(EP_Chassis) & 0x00ffUL;
    Upload_Package.Data[4] = FloatToInt16(V_Chassis) >> 8UL;
    Upload_Package.Data[5] = FloatToInt16(V_Chassis) & 0x00ffUL;
    Upload_Package.Data[6] = FloatToInt16(Capacitor_Percent) >> 8UL;
    Upload_Package.Data[7] = FloatToInt16(Capacitor_Percent) & 0x00ffUL;
}