//
// Created by Lao·Zhu on 2021/4/8.
//

#include "watchdog.h"
#include "system.h"
#include "datasave.h"

WWDG_HandleTypeDef hwwdg1;

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg) {
    DataSave_To_Flash(Watchdog_Reset);
    SoftReset();
}

void WatchDog_Task(void *pvParameters) {
    Watchdog_Config();
    while (1) {
        Delayms(20);
        HAL_WWDG_Refresh(&hwwdg1);
    }
}

void Watchdog_Config(void) {
    HAL_RCCEx_WWDGxSysResetConfig(RCC_WWDG1);
    __HAL_RCC_WWDG1_CLK_ENABLE();
    HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
    hwwdg1.Instance = WWDG1;
    hwwdg1.Init.Prescaler = WWDG_PRESCALER_32;  //Twwdg=(4096×2^WDGTB×(T[5:0]+1)) /RCC_PCLK3;
    hwwdg1.Init.Window = 126;                   //窗口值要大于0X40，就是大于64  4096*32*100/120000 = 109ms
    hwwdg1.Init.Counter = 127;                  //counter值要大于window，
    hwwdg1.Init.EWIMode = WWDG_EWI_ENABLE;
    while (HAL_WWDG_Init(&hwwdg1) != HAL_OK);
}