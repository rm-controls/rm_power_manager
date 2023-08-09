//
// Created by Lao·Zhu on 2021/3/14.
//

#include "key.h"
#include "gpio.h"

static Key_Press_t KeyPress_Structure, Key_Press_Digital_Structure;

Key_Press_t ReadKey_DigitalTube() {
    Key_Press_Digital_Structure.Last_Num = Key_Press_Digital_Structure.Num;
    if (HAL_GPIO_ReadPin(CENTER_KEY_PORT, CENTER_KEY_PIN) == GPIO_PIN_RESET)
        Key_Press_Digital_Structure.Num = User_Key;
    else
        Key_Press_Digital_Structure.Num = No_Key;
    return Key_Press_Digital_Structure;
}

Key_Press_t ReadKey() {
    KeyPress_Structure.Last_Num = KeyPress_Structure.Num;
    if (HAL_GPIO_ReadPin(UP_KEY_PORT, UP_KEY_PIN) == GPIO_PIN_RESET)
        KeyPress_Structure.Num = Up_Key;
    else if (HAL_GPIO_ReadPin(DOWN_KEY_PORT, DOWN_KEY_PIN) == GPIO_PIN_RESET)
        KeyPress_Structure.Num = Down_Key;
    else if (HAL_GPIO_ReadPin(LEFT_KEY_PORT, LEFT_KEY_PIN) == GPIO_PIN_RESET)
        KeyPress_Structure.Num = Left_Key;
    else if (HAL_GPIO_ReadPin(RIGHT_KEY_PORT, RIGHT_KEY_PIN) == GPIO_PIN_RESET)
        KeyPress_Structure.Num = Right_Key;
    else if (HAL_GPIO_ReadPin(CENTER_KEY_PORT, CENTER_KEY_PIN) == GPIO_PIN_RESET)
        KeyPress_Structure.Num = Center_Key;
    else
        KeyPress_Structure.Num = No_Key;
    return KeyPress_Structure;
}
