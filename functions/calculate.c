//
// Created by Lao·Zhu on 2021/2/4.
//

#include "stdlib.h"
#include "calculate.h"
#include "adc.h"
#include "filter.h"
#include "dac.h"
#include "port.h"
#include "system.h"
#include "referee.h"
#include "protect.h"
#include "FreeRTOS.h"
#include "task.h"

#define ADC_COEFFICIENT 3.0f / 4096.0f

extern TaskHandle_t PIDTask_Handler;
extern TaskHandle_t FSMTask_Handler;
Power_Calibrate_t Capacitor_Calibrate;
float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry, V_Chassis
, P_Chassis, P_Capacitor, EP_Chassis, W_Capacitor, Capacitor_Percent;
unsigned short I_CapOffset, I_ChaOffset;

void Referee_Power_Callback(void) {
    if (referee_data_.game_robot_status_.chassis_power_limit > 200)
        referee_data_.game_robot_status_.chassis_power_limit = 60;
}

unsigned char Verify_CalibrateCoefficient(void) {
    for (unsigned char counter = 10; counter < 120; counter += 10) {
        float verify_p =
            ((float) counter * Capacitor_Calibrate.coefficient[0] + Capacitor_Calibrate.coefficient[1])
                * (float) counter + Capacitor_Calibrate.coefficient[2];
        float error = verify_p - (float) counter;
        if (error > 30.0f || error < -30.0f)
            return 1;
    }
    return 0;
}

void ComplexPower_Calibrate(void) {              //Least square fitting of quadratic function
    vTaskSuspend(PIDTask_Handler);
    vTaskSuspend(FSMTask_Handler);
    Capacitor_Calibrate.coefficient[0] = 0;
    Capacitor_Calibrate.coefficient[1] = 1.0f;
    Capacitor_Calibrate.coefficient[2] = 0;
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_RESET);
    if (referee_avaiflag == 1) {
        float x[13], y[13], x_sum = 0, y_sum = 0, x2_sum = 0, x3_sum = 0, x4_sum = 0, xy_sum = 0, x2y_sum = 0;
        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
        for (unsigned char counter = 0; counter < 13; counter++) {
            float sample_xsum = 0, sample_ysum = 0;
            HAL_DAC_SetValue(&hdac1,
                             DAC_CHANNEL_1,
                             DAC_ALIGN_12B_R,
                             (unsigned short) (2730.0f * (float) (counter + 1) / V_Capacitor));     //10 ~ 130W
            for (unsigned char counterf = 0; counterf < 10; counterf++) {
                Delayms(60);
                sample_xsum += P_Capacitor;
                sample_ysum += referee_data_.power_heat_data_.chassis_power;
            }
            x[counter] = sample_xsum / 10.0f;
            y[counter] = sample_ysum / 10.0f;
            x_sum += x[counter];
            y_sum += y[counter];
            x2_sum += (x[counter] * x[counter]);
            x3_sum += (x[counter] * x[counter] * x[counter]);
            x4_sum += (x[counter] * x[counter] * x[counter] * x[counter]);
            xy_sum += (x[counter] * y[counter]);
            x2y_sum += (x[counter] * x[counter] * y[counter]);
        }
        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
        Capacitor_Calibrate.coefficient[1] = 0;
        for (int counter = 0; counter < 2000; ++counter) {
            Capacitor_Calibrate.coefficient[0] =
                (x2y_sum - x3_sum * Capacitor_Calibrate.coefficient[1] - x2_sum * Capacitor_Calibrate.coefficient[2])
                    / x4_sum;
            Capacitor_Calibrate.coefficient[1] =
                (xy_sum - x_sum * Capacitor_Calibrate.coefficient[2] - x3_sum * Capacitor_Calibrate.coefficient[0])
                    / x2_sum;
            Capacitor_Calibrate.coefficient[2] =
                (y_sum - x2_sum * Capacitor_Calibrate.coefficient[0] - x_sum * Capacitor_Calibrate.coefficient[1]) / 13;
        }
    } else {
        Capacitor_Calibrate.coefficient[0] = 0;
        Capacitor_Calibrate.coefficient[1] = 1.0f;
        Capacitor_Calibrate.coefficient[2] = 0;
    }
    vTaskResume(PIDTask_Handler);
    vTaskResume(FSMTask_Handler);
}

void SimplePower_Calibrate(void) {              // Least square fitting of first order function
    if (referee_avaiflag == 1) {
        float x[4], y[4], xy_sum = 0, x_ave, y_ave, x2_sum = 0, x_sum = 0, y_sum = 0;
        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
        for (unsigned char counter = 0; counter < 4; counter++) {
            float sample_local_sum = 0, sample_referee_sum = 0;
            HAL_DAC_SetValue(&hdac1,
                             DAC_CHANNEL_1,
                             DAC_ALIGN_12B_R,
                             (unsigned short) (2730.0f * (float) (counter + 1) / V_Capacitor));     //10 ~ 40W
            for (unsigned char counterf = 0; counterf < 5; counterf++) {
                Delayms(50);
                sample_local_sum += P_Capacitor;
                sample_referee_sum += referee_data_.power_heat_data_.chassis_power;
            }
            x[counter] = sample_local_sum / 5.0f;
            y[counter] = sample_referee_sum / 5.0f;
            xy_sum += (x[counter] * y[counter]);
            x2_sum += (x[counter] * x[counter]);
            x_sum += x[counter];
            y_sum += y[counter];
        }
        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
        x_ave = x_sum / 4.0f;
        y_ave = y_sum / 4.0f;
        Capacitor_Calibrate.coefficient[0] = 0;
        Capacitor_Calibrate.coefficient[1] = (xy_sum - 4 * x_ave * y_ave) / (x2_sum - 4 * x_ave * x_ave);
        Capacitor_Calibrate.coefficient[2] = y_ave - Capacitor_Calibrate.coefficient[1] * x_ave;
    } else {
        Capacitor_Calibrate.coefficient[0] = 0;
        Capacitor_Calibrate.coefficient[1] = 1.0f;
        Capacitor_Calibrate.coefficient[2] = 0;
    }
}

void Sensor_Config(void) {
    unsigned long capoffset_sum = 0, chaoffset_sum = 0;
    unsigned short ADCCap_Buffer[20], ADCCha_Buffer[20];
    Capacitor_Calibrate.coefficient[0] = 0;
    Capacitor_Calibrate.coefficient[1] = 1.0f;
    Capacitor_Calibrate.coefficient[2] = 0;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x00);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    Delayms(200);
    for (unsigned char counter = 0; counter < 20; counter++) {
        ADCCap_Buffer[counter] = ADC_FinalResult[0];
        ADCCha_Buffer[counter] = ADC_FinalResult[1];
        Delayms(20);
    }
    qsort(ADCCap_Buffer, 20, sizeof(ADCCap_Buffer[0]), Compare_UShort);
    qsort(ADCCha_Buffer, 20, sizeof(ADCCha_Buffer[0]), Compare_UShort);
    for (unsigned char counter = 0; counter < 10; counter++) {
        capoffset_sum += ADCCap_Buffer[5 + counter];
        chaoffset_sum += ADCCha_Buffer[5 + counter];
    }
    I_CapOffset = capoffset_sum / 10;
    I_ChaOffset = chaoffset_sum / 10;
    Delayms(200);
}

void Calculate_Power(void) {
    float tmp_capacitor_power = 0;
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

    W_Capacitor = 7.5f * V_Capacitor * V_Capacitor - 367.5f;        // Set 7V as 0% energy
    if (W_Capacitor < 0)
        W_Capacitor = 0;
    Capacitor_Percent = W_Capacitor / 1434.375f;                    // Set 15.5f as 100% energy

    if (I_Capacitor < 0.01f || I_Capacitor > 10.0f)
        I_Capacitor = 0;
    if (I_Chassis < 0.05f || I_Chassis > 10.0f)
        I_Chassis = 0;
    if (V_Capacitor < 0.01f || V_Capacitor > 25.0f)
        V_Capacitor = 0;
    if (V_Baterry < 0.01f || V_Baterry > 35.0f)
        V_Baterry = 0;
    if (V_Chassis < 0.01f || V_Chassis > 35.0f)
        V_Chassis = 0;

    P_ChassisF.Current_Value = I_Chassis * V_Chassis;
    P_Chassis = FirstOrder_Filter_Calculate(&P_ChassisF);

    P_CapacitorF.Current_Value = V_Baterry * I_Capacitor;
    tmp_capacitor_power = FirstOrder_Filter_Calculate(&P_CapacitorF);
    P_Capacitor =
        (tmp_capacitor_power * Capacitor_Calibrate.coefficient[0] + Capacitor_Calibrate.coefficient[1])
            * tmp_capacitor_power + Capacitor_Calibrate.coefficient[2];
    if (P_Capacitor < 0)
        P_Capacitor = 0;
}