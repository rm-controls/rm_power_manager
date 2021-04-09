//
// Created by Lao·Zhu on 2021/4/8.
//

#ifndef PROGRAM_HARDWARE_WATCHDOG_H_
#define PROGRAM_HARDWARE_WATCHDOG_H_

#include "stm32h7xx_hal.h"

extern WWDG_HandleTypeDef hwwdg1;

void Watchdog_Config(void);
void WatchDog_Task(void *pvParameters);

#endif //PROGRAM_HARDWARE_WATCHDOG_H_
