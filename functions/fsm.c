//
// Created by Lao·Zhu on 2021/4/5.
//

#include "rtc.h"
#include "fsm.h"
#include "system.h"
#include "stm32h7xx_hal.h"
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "pid.h"
#include "referee.h"
#include "calculate.h"
#include "string.h"
#include "watchdog.h"
#include "protect.h"

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
    Sensor_Config();
    SimplePower_Calibrate();
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);

    if (Verify_CalibrateCoefficient() == 1) {
        Capacitor_Calibrate.coefficient[0] = 0;
        Capacitor_Calibrate.coefficient[1] = 1.0f;
        Capacitor_Calibrate.coefficient[2] = 0;
    }

    xSemaphoreGive(Calibrate_Semaphore);
    Delayms(10);
    xTaskCreate(WatchDog_Task, "WatchDog", 128, NULL, 1, &WatchdogTask_Handler);

    while (1) {
        switch (FSM_Status.FSM_Mode) {
            case Normal_Mode:
                if (FSM_Status.Charge_Mode != Zero_Power_Charge || V_Capacitor <= 15.2f)
                    FSM_Status.Charge_Mode = FSM_Status.uCharge_Mode;
                FSM_Status.Expect_Mode = FSM_Status.uExpect_Mode;
                FSM_Status.Typology_Mode = Chassis_With_Charge;
                break;
            case OverPower_Mode:
                if (FSM_Status.Charge_Mode != Zero_Power_Charge || V_Capacitor <= 15.2f)
                    FSM_Status.Charge_Mode = Full_Power_Charge;
                FSM_Status.Expect_Mode = OverPower_Expect;
                FSM_Status.Typology_Mode = Boost_With_Charge;
                break;
            case Halt_Mode:FSM_Status.Charge_Mode = Zero_Power_Charge;
                FSM_Status.Expect_Mode = ZeroPower_Expect;
                FSM_Status.Typology_Mode = All_Off;
                break;
            case NoCharge_Mode:FSM_Status.Charge_Mode = Zero_Power_Charge;
                FSM_Status.Expect_Mode = FullPower_Expect;
                FSM_Status.Typology_Mode = Only_DeliverChassis;
                break;
            case SucapTest_Mode:
                if (FSM_Status.Charge_Mode != Zero_Power_Charge || V_Capacitor <= 15.2f)
                    FSM_Status.Charge_Mode = Full_Power_Charge;
                FSM_Status.Expect_Mode = ZeroPower_Expect;
                FSM_Status.Typology_Mode = Only_Charge;
                break;
        }
        if (V_Capacitor > 15.5f ||
            (referee_data_.power_heat_data_.chassis_power_buffer <= 5 && referee_avaiflag == 1))
            FSM_Status.Charge_Mode = Zero_Power_Charge;
        switch (FSM_Status.Charge_Mode) {
            case Zero_Power_Charge:vTaskSuspend(PIDTask_Handler);
                HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
                break;
            case Const_Power_Charge:
                if (referee_data_.power_heat_data_.chassis_power_buffer <= 20)
                    PID_Capacitor.User = (float) FSM_Status.Charge_Power - 10.0f;
                else if (referee_data_.power_heat_data_.chassis_power_buffer <= 30)
                    PID_Capacitor.User = (float) FSM_Status.Charge_Power - 5.0f;
                else if (referee_data_.power_heat_data_.chassis_power_buffer >= 50)
                    PID_Capacitor.User = FSM_Status.Charge_Power;
                vTaskResume(PIDTask_Handler);
                HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
                break;
            case Proportional_Charge:
                if (referee_data_.power_heat_data_.chassis_power_buffer <= 10)
                    PID_Capacitor.User = (float) (referee_data_.game_robot_status_.chassis_power_limit)
                        * FSM_Status.P_Charge * 0.7f;
                else if (referee_data_.power_heat_data_.chassis_power_buffer <= 20)
                    PID_Capacitor.User = (float) (referee_data_.game_robot_status_.chassis_power_limit)
                        * FSM_Status.P_Charge * 0.8f;
                else if (referee_data_.power_heat_data_.chassis_power_buffer <= 30)
                    PID_Capacitor.User = (float) (referee_data_.game_robot_status_.chassis_power_limit)
                        * FSM_Status.P_Charge * 0.9f;
                else if (referee_data_.power_heat_data_.chassis_power_buffer >= 50)
                    PID_Capacitor.User = (float) (referee_data_.game_robot_status_.chassis_power_limit)
                        * FSM_Status.P_Charge;
                vTaskResume(PIDTask_Handler);
                HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
                break;
            case Full_Power_Charge:
                if (referee_data_.power_heat_data_.chassis_power_buffer <= 10)
                    PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 15.0f;
                else if (referee_data_.power_heat_data_.chassis_power_buffer <= 20)
                    PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 10.0f;
                else if (referee_data_.power_heat_data_.chassis_power_buffer <= 30)
                    PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit - 5.0f;
                else if (referee_data_.power_heat_data_.chassis_power_buffer >= 50)
                    PID_Capacitor.User = (float) referee_data_.game_robot_status_.chassis_power_limit + 5.0f;
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
            case FullPower_Expect:EP_Chassis = (float) referee_data_.game_robot_status_.chassis_power_limit;
                break;
            case RemainPower_Expect:
                EP_Chassis = (float) (referee_data_.game_robot_status_.chassis_power_limit
                    - FSM_Status.Charge_Power);
                break;
            case OverPower_Expect:EP_Chassis = FSM_Status.Max_Power;
                break;
            case ZeroPower_Expect:EP_Chassis = 0;
                break;
        }
        if (Last_FSM_Status.Typology_Mode != FSM_Status.Typology_Mode) {
            switch (FSM_Status.Typology_Mode) {
                case Boost_With_Charge:HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_RESET);
                    Delayms(50);
                    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
                    break;
                case Only_DeliverChassis:
                case Chassis_With_Charge:HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_RESET);
                    Delayms(50);
                    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
                    break;
                case Only_Charge:
                case All_Off:HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
                    break;
            }
        }
        Last_FSM_Status.Typology_Mode = FSM_Status.Typology_Mode;
        Last_FSM_Status.FSM_Mode = FSM_Status.FSM_Mode;
        Delayms(5);
    }
}
