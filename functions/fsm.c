//
// Created by Lao·Zhu on 2021/4/5.
//

#include "fsm.h"
#include "system.h"
#include "stm32h7xx_hal.h"
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "pid.h"
#include "refree.h"
#include "calculate.h"
#include "string.h"
#include "watchdog.h"

extern TaskHandle_t PIDTask_Handler;
FSM_Status_t FSM_Status, Last_FSM_Status;
extern SemaphoreHandle_t Calibrate_Semaphore;
extern TaskHandle_t ProtectTask_Handler;
TaskHandle_t WatchdogTask_Handler;

void FSM_Task(void *pvParameters) {
    memset(&Last_FSM_Status, 0x00, sizeof(FSM_Status_t));

    xSemaphoreTake(Calibrate_Semaphore, 0xFFFFFFFFUL);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    Calibrate_Powerl();
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    xSemaphoreGive(Calibrate_Semaphore);

    Delayms(10);

    xTaskCreate(WatchDog_Task, "WatchDogTask", 128, NULL, 1, &WatchdogTask_Handler);

    while (1) {
        switch (FSM_Status.FSM_Mode) {
            case Normal_Mode:
                if (FSM_Status.Charge_Mode != Zero_Power_Charge || V_Capacitor <= 15.0)
                    FSM_Status.Charge_Mode = FSM_Status.uCharge_Mode;
                FSM_Status.Expect_Mode = FSM_Status.uExpect_Mode;
                FSM_Status.Typology_Mode = PMOS_With_Charge;
                break;
            case OverPower_Mode:
                if (Last_FSM_Status.FSM_Mode == Normal_Mode) {
                    vTaskSuspend(PIDTask_Handler);
                    vTaskSuspend(ProtectTask_Handler);
                    Calibrate_Powerh();
                    vTaskResume(PIDTask_Handler);
                    vTaskResume(ProtectTask_Handler);
                }
                if (FSM_Status.Charge_Mode != Zero_Power_Charge || V_Capacitor <= 15.0)
                    FSM_Status.Charge_Mode = Full_Power_Charge;
                FSM_Status.Expect_Mode = OverPower_Expect;
                FSM_Status.Typology_Mode = Only_Charge;
                break;
            case Halt_Mode:FSM_Status.Charge_Mode = Zero_Power_Charge;
                FSM_Status.Expect_Mode = FSM_Status.uExpect_Mode;
                FSM_Status.Typology_Mode = All_Off;
                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
                break;
            case Transition_Mode:
                if (FSM_Status.Charge_Mode != Zero_Power_Charge || V_Capacitor <= 15.0)
                    FSM_Status.Charge_Mode = Full_Power_Charge;
                FSM_Status.Expect_Mode = FSM_Status.uExpect_Mode;
                FSM_Status.Typology_Mode = Only_Charge;
                break;
        }
        if (V_Capacitor >= 15.5f || referee_data_.power_heat_data_.chassis_power_buffer <= 5)
            FSM_Status.Charge_Mode = Zero_Power_Charge;
        switch (FSM_Status.Charge_Mode) {
            case Zero_Power_Charge:vTaskSuspend(PIDTask_Handler);
                HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
                break;
            case Const_Power_Charge:PID_Capacitor.User = FSM_Status.Charge_Power;
                vTaskResume(PIDTask_Handler);
                HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
                break;
            case Proportional_Charge:
                PID_Capacitor.User =
                    (float) referee_data_.game_robot_status_.chassis_power_limit
                        * FSM_Status.P_Charge;
                vTaskResume(PIDTask_Handler);
                HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
                break;
            case Full_Power_Charge:
//                PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 4.0f;
                vTaskResume(PIDTask_Handler);
                HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
                break;
            case Remain_Power_Charge:
                PID_Capacitor.User = (float) (referee_data_.game_robot_status_.chassis_power_limit
                    - FSM_Status.Expect_Power);
                vTaskResume(PIDTask_Handler);
                HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
                break;
        }
        switch (FSM_Status.Expect_Mode) {
            case Const_Power_Expect:EP_Chassis = FSM_Status.Expect_Power;
                break;
            case Proportional_Expect:
                EP_Chassis = (float) referee_data_.game_robot_status_.chassis_power_limit
                    * FSM_Status.P_Expect;
                break;
            case Full_Power_Expect:EP_Chassis = (float) referee_data_.game_robot_status_.chassis_power_limit;
                break;
            case Remain_Power_Expect:
                EP_Chassis = (float) (referee_data_.game_robot_status_.chassis_power_limit
                    - FSM_Status.Charge_Power);
                break;
            case OverPower_Expect:EP_Chassis = FSM_Status.Max_Power;
                break;
        }
        switch (FSM_Status.Typology_Mode) {
            case Only_Charge:HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_RESET);
                Delayms(50);
                HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
                break;
            case Only_PMOS:
            case PMOS_With_Charge:HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_RESET);
                Delayms(50);
                HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
                break;
            case All_Off:HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
                break;
        }
        Last_FSM_Status.FSM_Mode = FSM_Status.FSM_Mode;
        Delayms(5);
    }
}
