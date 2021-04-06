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
#include "fsm.h"
#include "pid.h"

#define ADC_COEFFICIENT 3.0f / 4096.0f

Power_Calibrate_t Capacitor_Calibratel, Capacitor_Calibrateh, Chassis_Calibrate;
float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry, V_Chassis
, P_Chassis, P_Capacitor, EP_Chassis, W_Capacitor;
unsigned short I_CapOffset, I_ChaOffset;

void Refree_Power_Callback(void) {
    if (FSM_Status.Charge_Mode == Full_Power_Charge) {
        Capacitor_Calibrateh.Pd[0] = P_Capacitor;
        Capacitor_Calibrateh.Pr[0] = referee_data_.power_heat_data_.chassis_power;
        Capacitor_Calibrateh.Iw[0] = I_Capacitor;
        Capacitor_Calibrateh.Rw = (Capacitor_Calibrateh.Pr[0] - Capacitor_Calibrateh.Pd[0])
            / (Capacitor_Calibrateh.Iw[0] * Capacitor_Calibrateh.Iw[0]);
        if (referee_data_.power_heat_data_.chassis_power_buffer <= 30)
            PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 10.0f;
        if (referee_data_.power_heat_data_.chassis_power_buffer == 60)
            PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit;
    }
}

void Calibrate_Power(void) {
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (unsigned short) (5460.0f / V_Capacitor)); //20W
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    Delayms(500);
    Capacitor_Calibratel.Pd[0] = P_Capacitor;
    Capacitor_Calibratel.Pr[0] = referee_data_.power_heat_data_.chassis_power;
    Capacitor_Calibratel.Iw[0] = I_Capacitor;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (unsigned short) (10920.0f / V_Capacitor)); //40W
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    Delayms(500);
    Capacitor_Calibratel.Pd[1] = P_Capacitor;
    Capacitor_Calibratel.Pr[1] = referee_data_.power_heat_data_.chassis_power;
    Capacitor_Calibratel.Iw[1] = I_Capacitor;
    Capacitor_Calibratel.Rw = (Capacitor_Calibratel.Pr[0] - Capacitor_Calibratel.Pd[0])
        / (Capacitor_Calibratel.Iw[0] * Capacitor_Calibratel.Iw[0]);
    Capacitor_Calibratel.Rw = (Capacitor_Calibratel.Rw + (Capacitor_Calibratel.Pr[1] - Capacitor_Calibratel.Pd[1])
        / (Capacitor_Calibratel.Iw[1] * Capacitor_Calibratel.Iw[1])) * 0.5f;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
}

void Sensor_Config(void) {
    Capacitor_Calibratel.Rw = 0;
    Chassis_Calibrate.Rw = 0;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x00);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_Delay(500);
    I_CapOffset = ADC_FinalResult[0];
    I_ChaOffset = ADC_FinalResult[1];
    HAL_Delay(500);
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

    if (FSM_Status.Charge_Mode != Full_Power_Charge) {
        P_CapacitorF.Current_Value = V_Baterry * I_Capacitor;
        P_Capacitor = FirstOrder_Filter_Calculate(&P_CapacitorF) + I_Capacitor * I_Capacitor * Capacitor_Calibratel.Rw;
    } else {
        P_CapacitorF.Current_Value = V_Baterry * I_Capacitor;
        P_Capacitor = FirstOrder_Filter_Calculate(&P_CapacitorF) + I_Capacitor * I_Capacitor * Capacitor_Calibrateh.Rw;
    }

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