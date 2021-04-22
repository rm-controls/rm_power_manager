//
// Created by Lao·Zhu on 2021/3/14.
//

#include "key.h"
#include "stm32h7xx_hal.h"
#include "system.h"
#include "port.h"

Key_Num_e ReadKey() {
    if (HAL_GPIO_ReadPin(UP_KEY_PORT, UP_KEY_PIN) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(UP_KEY_PORT, UP_KEY_PIN) == GPIO_PIN_RESET)
            Delayms(1);
        return Up_Key;
    }
    if (HAL_GPIO_ReadPin(DOWN_KEY_PORT, DOWN_KEY_PIN) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(DOWN_KEY_PORT, DOWN_KEY_PIN) == GPIO_PIN_RESET)
            Delayms(1);
        return Down_Key;
    }
    if (HAL_GPIO_ReadPin(LEFT_KEY_PORT, LEFT_KEY_PIN) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(LEFT_KEY_PORT, LEFT_KEY_PIN) == GPIO_PIN_RESET)
            Delayms(1);
        return Left_Key;
    }
    if (HAL_GPIO_ReadPin(RIGHT_KEY_PORT, RIGHT_KEY_PIN) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(RIGHT_KEY_PORT, RIGHT_KEY_PIN) == GPIO_PIN_RESET)
            Delayms(1);
        return Right_Key;
    }
    if (HAL_GPIO_ReadPin(CENTER_KEY_PORT, CENTER_KEY_PIN) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(CENTER_KEY_PORT, CENTER_KEY_PIN) == GPIO_PIN_RESET)
            Delayms(1);
        return Center_Key;
    }
    return 0;
}
