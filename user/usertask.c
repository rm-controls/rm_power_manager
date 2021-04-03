//
// Created by Lao·Zhu on 2021/2/4.
//

#include "main.h"

void UserTask(void *pvParameters) {
    float theta = 0;
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    PID_Capacitor.User = 40;
    while (1) {
//        if (V_Capacitor >= 15.0f) {
//            HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_RESET);
//            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
//            while (V_Capacitor < 7.0f)
//                Delayms(10);
//        } else if (V_Capacitor <= 7.0f) {
//            HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
//            while (V_Capacitor > 15.0f)
//                Delayms(10);
//        }
        theta = theta + 0.05f;
        PID_Capacitor.User = (sinf(theta) + 2) * 15;
        Delayms(10);
    }
}
