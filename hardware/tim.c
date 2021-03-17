#include "tim.h"

TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

void TIM_Config(void) {
    htim10.Instance = TIM10;
    htim10.Init.Prescaler = 16799;//168000000 / 16800 = 10000;
    htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim10.Init.Period = 4999;   //10000 / 5000 = 2Hz
    htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
    htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    while (HAL_TIM_Base_Init(&htim10));
    htim11.Instance = TIM11;
    htim11.Init.Prescaler = 1679;          //168000000 / 1680 = 100000;
    htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim11.Init.Period = 99;                //100000 / 100 = 1000Hz
    htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
    htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    while (HAL_TIM_Base_Init(&htim11));
    HAL_TIM_Base_Start_IT(&htim10);
    HAL_TIM_Base_Start_IT(&htim11);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle) {
    if (tim_baseHandle->Instance == TIM10) {
        __HAL_RCC_TIM10_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    } else if (tim_baseHandle->Instance == TIM11) {
        __HAL_RCC_TIM11_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle) {
    if (tim_baseHandle->Instance == TIM10) {
        __HAL_RCC_TIM10_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
    } else if (tim_baseHandle->Instance == TIM11) {
        __HAL_RCC_TIM11_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    }
}
