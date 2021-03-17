//
// Created by Lao·Zhu on 2021/2/4.
//

#include "main.h"

void UserTask(void *pvParameters) {
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    PID_Capacitor_P.User = 40;
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    while (1) {
        if (V_Capacitor > 6.5f)
            break;
        Delayms(10);
    }
    while (1) {
        if (V_Capacitor >= 15.0f) {
            HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
            while (V_Capacitor < 7.0f)
                Delayms(10);
        } else if (V_Capacitor <= 5.0f) {
            HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
            while (V_Capacitor > 15.0f)
                Delayms(10);
        } else
            Delayms(10);
    }
}
