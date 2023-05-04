//
// Created by Lao·Zhu on 2021/4/5.
//

#include "protect.h"
#include "fsm.h"
#include "calculate.h"
#include "system.h"
#include "datasave.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stm32h7xx_hal.h"
#include "usart.h"
#include "referee.h"
#include "sysinfo_form.h"
#include "usertask.h"

extern TaskHandle_t ProtectTask_Handler;
unsigned short referee_avaiflag = 0, referee_time_counter = 0;
extern SemaphoreHandle_t Calibrate_Semaphore;
extern FSM_Status_t Last_FSM_Status;
extern unsigned char UART1_IT_Flag;
unsigned char overcurrent_flag = 0;
unsigned char overcurrent_counter_flag = 0;
unsigned short overcurrent_counter = 0;

void Protect_Task(void *pvParameters) {
    Delayms(10);
    xSemaphoreTake(Calibrate_Semaphore, 0xFFFFFFFFUL);
    xSemaphoreGive(Calibrate_Semaphore);
    Delayms(100);
    float I_boost_max = (float) (FSM_Status.Max_Power) / 24.0f;
    while (1) {
        referee_time_counter++;
        if (UART1_IT_Flag != HAL_OK) {
            UART1_IT_Flag = HAL_OK;
            UART1_Config();
        }

        if ((FSM_Status.FSM_Mode == Normal_Mode || FSM_Status.FSM_Mode == NoCharge_Mode)
            && I_Chassis >= 9.0f) {
            Delayms(200);
            if (I_Chassis >= 9.0f) {
                FSM_Status.FSM_Mode = Halt_Mode;
                overcurrent_flag = 1;
                overcurrent_counter_flag = 1;
            }
        } else if (FSM_Status.FSM_Mode == OverPower_Mode && I_Chassis >= I_boost_max) {
            Delayms(200);
            if (I_Chassis >= I_boost_max) {
                FSM_Status.FSM_Mode = Halt_Mode;
                overcurrent_flag = 1;
                overcurrent_counter_flag = 1;
            }
        }
        if (overcurrent_counter_flag == 1 && overcurrent_flag == 1) {
            overcurrent_counter++;
            if (overcurrent_counter == 1000) {
                FSM_Status.FSM_Mode = NoCharge_Mode;
                overcurrent_counter = 0;
                overcurrent_counter_flag = 0;
                overcurrent_flag = 0;
            }
        }
        if (V_Baterry <= 18.0f) {
            Delayms(100);
            if (V_Baterry <= 18.0f) {
                overcurrent_flag = 0;
                FSM_Status.FSM_Mode = Halt_Mode;
            }
        } else if (FSM_Status.FSM_Mode == Halt_Mode && V_Baterry > 20.0f && overcurrent_flag == 0
                   && Setting_OptiSchemes != SucapTest_Optimized) {
            Delayms(100);
            if (Setting_OptiSchemes != SucapTest_Optimized && FSM_Status.FSM_Mode == Halt_Mode) {
                vTaskPrioritySet(ProtectTask_Handler, 1);
                Delayms(100);
                DataSave_To_Flash(RePowerOn_Reset);
                SoftReset();
            }
        }
        if (Last_FSM_Status.FSM_Mode == OverPower_Mode && Capacitor_Percent < 0.08f) {
            FSM_Status.FSM_Mode = Normal_Mode;
        }
        if (referee_time_counter >= 1000) {
            referee_avaiflag = 0;
            referee_time_counter = 0;
            referee_data_.game_robot_status_.chassis_power_limit = 50;
            referee_data_.power_heat_data_.chassis_power_buffer = 60;
            referee_data_.game_status_.game_progress = 0;
            HAL_UART_MspDeInit(&huart2);
            HAL_UART_MspInit(&huart2);
            UART2_Config();
        }
        Delayms(1);
    }
}
