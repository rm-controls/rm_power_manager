//
// Created by Lao·Zhu on 2021/3/14.
//

#ifndef STM32_HARDWARE_KEY_H_
#define STM32_HARDWARE_KEY_H_

typedef enum {
    Up_Key = 1,
    Down_Key = 2,
    Left_Key = 3,
    Right_Key = 4,
    Center_Key = 5,
} Key_Num_e;

Key_Num_e ReadKey();

#endif //STM32_HARDWARE_KEY_H_
