#include "tim.h"

TIM_HandleTypeDef htim7;

void tim7_config(void) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler;
    uint32_t uwPrescalerValue;
    uint32_t pFLatency;

    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    else
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();

    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
    htim7.Instance = TIM7;
    htim7.Init.Period = (1000000U / 20U) - 1U;
    htim7.Init.Prescaler = uwPrescalerValue;
    htim7.Init.ClockDivision = 0;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&htim7) == HAL_OK)
        HAL_TIM_Base_Start_IT(&htim7);
    else
        error_handler(__FILE__, __LINE__);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle) {
    if (tim_baseHandle->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM7_IRQn, TIM7_GLOBAL_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle) {
    if (tim_baseHandle->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM7_IRQn);
    }
}