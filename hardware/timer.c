//
// Created by Lao·Zhu on 2021/4/3.
//

#include "timer.h"

TIM_HandleTypeDef htim7;

void TIM7_Config(void) {
    __HAL_RCC_TIM7_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM7_IRQn, 2, 0U);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);

    htim7.Instance = TIM7;
    htim7.Init.Period = 128 - 1U;           // 128MHz / 12800 = 1MHz
    htim7.Init.Prescaler = 100 - 1;         // 1MHz / 100 = 10KHz
    htim7.Init.ClockDivision = 0;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    while (HAL_TIM_Base_Init(&htim7) != HAL_OK);
    HAL_TIM_Base_Start_IT(&htim7);
}
