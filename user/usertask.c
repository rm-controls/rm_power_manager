//
// Created by Lao·Zhu on 2021/2/4.
//

#include "main.h"

void UserTask(void *pvParameters) {
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    PID_Capacitor.User = 20;
    EP_Chassis = 20;
    while (1) {
        if (V_Capacitor >= 15.0f) {
            HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
            EP_Chassis = 100;
            PID_Capacitor.User = 40;
            while (1) {
                if (V_Capacitor <= 7.0f)break;
                Delayms(10);
            }
        } else if (V_Capacitor <= 7.0f) {
            HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
            EP_Chassis = 20;
            PID_Capacitor.User = 20;
            while (1) {
                if (V_Capacitor >= 15.0f)break;
                Delayms(10);
            }
        }
    }
}
