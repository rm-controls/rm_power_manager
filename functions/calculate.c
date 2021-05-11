//
// Created by Lao·Zhu on 2021/2/4.
//

#include "calculate.h"
#include "adc.h"
#include "filter.h"
#include "dac.h"
#include "port.h"
#include "system.h"
#include "referee.h"
#include "fsm.h"
#include "pid.h"
#include "protect.h"

#define ADC_COEFFICIENT 3.0f / 4096.0f

Power_Calibrate_t Capacitor_Calibratel, Capacitor_Calibrateh;
float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry, V_Chassis
, P_Chassis, P_Capacitor, EP_Chassis, W_Capacitor;
unsigned short I_CapOffset, I_ChaOffset;

void Refree_Power_Callback(void) {
    if (FSM_Status.Charge_Mode == Full_Power_Charge) {
        if (referee_data_.power_heat_data_.chassis_power_buffer <= 10)
            PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 20.0f;
        else if (referee_data_.power_heat_data_.chassis_power_buffer <= 20)
            PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 15.0f;
        else if (referee_data_.power_heat_data_.chassis_power_buffer <= 30)
            PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 10.0f;
        else if (referee_data_.power_heat_data_.chassis_power_buffer >= 50)
            PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 4.0f;
    } else if (FSM_Status.Charge_Mode == Const_Power_Charge) {
        if (referee_data_.power_heat_data_.chassis_power_buffer <= 10)
            PID_Capacitor.User = 0;
        else if (referee_data_.power_heat_data_.chassis_power_buffer <= 10)
            PID_Capacitor.User = (float) FSM_Status.Charge_Power - 15.0f;
        else if (referee_data_.power_heat_data_.chassis_power_buffer <= 20)
            PID_Capacitor.User = (float) FSM_Status.Charge_Power - 10.0f;
        else if (referee_data_.power_heat_data_.chassis_power_buffer <= 30)
            PID_Capacitor.User = (float) FSM_Status.Charge_Power - 5.0f;
        else if (referee_data_.power_heat_data_.chassis_power_buffer >= 50)
            PID_Capacitor.User = FSM_Status.Charge_Power;
    }
}

void Calibrate_Powerh(void) {
    if (referee_avaiflag == 1) {
        HAL_DAC_SetValue(&hdac1,
                         DAC_CHANNEL_1,
                         DAC_ALIGN_12B_R,
                         (unsigned short) (273.0f * (float) referee_data_.game_robot_status_.chassis_power_limit
                             / V_Capacitor));
        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
        Delayms(200);
        Capacitor_Calibrateh.Pd[0] = P_Capacitor;
        Capacitor_Calibrateh.Pr[0] = referee_data_.power_heat_data_.chassis_power;
        Capacitor_Calibrateh.Iw[0] = I_Capacitor;
        HAL_DAC_SetValue(&hdac1,
                         DAC_CHANNEL_1,
                         DAC_ALIGN_12B_R,
                         (unsigned short) (273.0f
                             * ((float) referee_data_.game_robot_status_.chassis_power_limit - 10.0f)
                             / V_Capacitor));
        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
        Delayms(200);
        Capacitor_Calibrateh.Pd[1] = P_Capacitor;
        Capacitor_Calibrateh.Pr[1] = referee_data_.power_heat_data_.chassis_power;
        Capacitor_Calibrateh.Iw[1] = I_Capacitor;
        Capacitor_Calibrateh.Rw = (Capacitor_Calibrateh.Pr[0] - Capacitor_Calibrateh.Pd[0])
            / (Capacitor_Calibrateh.Iw[0] * Capacitor_Calibrateh.Iw[0]);
        Capacitor_Calibrateh.Rw = (Capacitor_Calibrateh.Rw + (Capacitor_Calibrateh.Pr[1] - Capacitor_Calibrateh.Pd[1])
            / (Capacitor_Calibrateh.Iw[1] * Capacitor_Calibrateh.Iw[1])) * 0.5f;
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
    } else
        Capacitor_Calibrateh.Rw = 0.5f;
}

void Calibrate_Powerl(void) {
    if (referee_avaiflag == 1) {
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
    } else
        Capacitor_Calibratel.Rw = 0.5f;
}

void Sensor_Config(void) {
    Capacitor_Calibratel.Rw = 0;
    Capacitor_Calibrateh.Rw = 0;
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

    if (I_Capacitor < 0.05f || I_Capacitor > 10.0f)
        I_Capacitor = 0;
    if (I_Chassis < 0.05f || I_Chassis > 10.0f)
        I_Chassis = 0;
    if (V_Capacitor < 0.1f || V_Capacitor > 25.0f)
        V_Capacitor = 0;
    if (V_Baterry < 0.1f || V_Baterry > 35.0f)
        V_Baterry = 0;
    if (V_Chassis < 0.1f || V_Chassis > 25.0f)
        V_Chassis = 0;

    P_ChassisF.Current_Value = I_Chassis * V_Chassis;
    P_Chassis = FirstOrder_Filter_Calculate(&P_ChassisF);

    if (FSM_Status.Charge_Mode != Full_Power_Charge) {
        P_CapacitorF.Current_Value = V_Baterry * I_Capacitor;
        P_Capacitor = FirstOrder_Filter_Calculate(&P_CapacitorF) + I_Capacitor * I_Capacitor * Capacitor_Calibratel.Rw;
    } else {
        P_CapacitorF.Current_Value = V_Baterry * I_Capacitor;
        P_Capacitor = FirstOrder_Filter_Calculate(&P_CapacitorF) + I_Capacitor * I_Capacitor * Capacitor_Calibrateh.Rw;
    }

    if (P_Chassis < 0.1f || P_Chassis > 250.0f)
        P_Chassis = 0;
    if (P_Capacitor < 0.1f || P_Capacitor > 200.0f)
        P_Capacitor = 0;
}