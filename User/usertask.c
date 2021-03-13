//
// Created by Lao·Zhu on 2021/2/4.
//

#include "main.h"

void UserTask(void *pvParameters) {
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 128);
    while (1) {
//        if (V_Capacitor >= 15.0f) {
//            HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
//            HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_RESET);
//            PID_Capacitor.User = 60;
//            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
//            while (V_Capacitor > 7.0f)
//                Delayms(10);
//        } else if (V_Capacitor <= 7.0f) {
//            HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_RESET);
//            Delayms(1);
//            HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
//            PID_Capacitor.User = 30;
//            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
//            while (V_Capacitor < 15.0f)
        PID_Capacitor_C.User = 1;
        Delayms(1000);
        PID_Capacitor_C.User = 2;
        Delayms(1000);
//    }
    }
}
