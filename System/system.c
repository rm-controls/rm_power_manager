//
// Created by Lao·Zhu on 2021/1/20.
//

#include "system.h"
#include "stm32f4xx_hal.h"

TIM_HandleTypeDef htim4;

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock = 0;
    uint32_t uwPrescalerValue = 0;
    uint32_t pFLatency;
    HAL_NVIC_SetPriority(TIM4_IRQn, TickPriority, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
    __HAL_RCC_TIM4_CLK_ENABLE();
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
    uwTimclock = 2 * HAL_RCC_GetPCLK1Freq();
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
    htim4.Instance = TIM4;
    htim4.Init.Period = (1000000U / 1000U) - 1U;
    htim4.Init.Prescaler = uwPrescalerValue;
    htim4.Init.ClockDivision = 0;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    if (HAL_TIM_Base_Init(&htim4) == HAL_OK) {
        return HAL_TIM_Base_Start_IT(&htim4);
    }
    return HAL_ERROR;
}

void HAL_SuspendTick(void) {
    __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void) {
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    while (HAL_RCC_OscConfig(&RCC_OscInitStruct));
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    while (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5));
}

unsigned short FloatToInt16(float data0) {
    if (data0 == 0)
        return 0;
    unsigned short fInt16;
    unsigned int *pfp32 = ((unsigned int *) &data0);
    fInt16 = ((*pfp32 >> 16) & 0x8000) |
        ((((*pfp32 >> 23) - 0x7f + 0x0f) & 0x1f) << 10) |
        ((*pfp32 >> 13) & 0x3ff);
    return fInt16;
}

float Int16ToFloat(unsigned short data0) {
    if (data0 == 0)
        return 0;
    float *fp32;
    unsigned int fInt32 = ((data0 & 0x8000) << 16) |
        (((((data0 >> 10) & 0x1f) - 0x0f + 0x7f) & 0xff) << 23)
        | ((data0 & 0x03FF) << 13);
    fp32 = (float *) &fInt32;
    return *fp32;
}
