#include "tim.h"

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

void TIM_Config(void) {
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 8399;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = 199;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    while (HAL_TIM_Base_Init(&htim6));
//    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
//    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//    while (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig));
    htim7.Instance = TIM7;
    htim7.Init.Prescaler = 8399;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = 49;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    while (HAL_TIM_Base_Init(&htim7));
//    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
//    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//    while (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig));
    htim10.Instance = TIM10;
    htim10.Init.Prescaler = 83;
    htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim10.Init.Period = 65535;
    htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
    htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    while (HAL_TIM_Base_Init(&htim10));
    htim11.Instance = TIM11;
    htim11.Init.Prescaler = 83;
    htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim11.Init.Period = 65535;
    htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
    htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    while (HAL_TIM_Base_Init(&htim11));
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_Base_Start_IT(&htim7);
    HAL_TIM_Base_Start_IT(&htim10);
    HAL_TIM_Base_Start_IT(&htim11);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle) {

    if (tim_baseHandle->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    } else if (tim_baseHandle->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    } else if (tim_baseHandle->Instance == TIM10) {
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
    if (tim_baseHandle->Instance == TIM6) {
        __HAL_RCC_TIM6_CLK_DISABLE();
    } else if (tim_baseHandle->Instance == TIM7) {
        __HAL_RCC_TIM7_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM7_IRQn);
    } else if (tim_baseHandle->Instance == TIM10) {
        __HAL_RCC_TIM10_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
    } else if (tim_baseHandle->Instance == TIM11) {
        __HAL_RCC_TIM11_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    }
}
