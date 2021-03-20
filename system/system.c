//
// Created by Lao·Zhu on 2021/1/20.
//

#include "system.h"
#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

static unsigned int Facus = 0;
static unsigned int Facms = 0;
TIM_HandleTypeDef htim4;

void Delayus(unsigned int xus) {
    unsigned int Ticks, Time_Old, Time_Now, Time_Count = 0;
    unsigned int Reload = SysTick->LOAD;
    Ticks = xus * Facus;
    Time_Old = SysTick->VAL;
    while (1) {
        Time_Now = SysTick->VAL;
        if (Time_Now != Time_Old) {
            if (Time_Now < Time_Old)
                Time_Count += Time_Old - Time_Now;
            else
                Time_Count += Reload - Time_Now + Time_Old;
            Time_Old = Time_Now;
            if (Time_Count >= Ticks)
                break;
        }
    }
}

void Delayms(unsigned int xms) {
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) {
        if (xms >= Facms) {
            vTaskDelay(xms / Facms);
        } else {
            xms %= Facms;
            Delayus(xms * 1000);
        }
    } else
        Delayus(xms * 1000);
}

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_SYSCFG_DisableVREFBUF();
    HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_ENABLE);
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler;
    uint32_t uwPrescalerValue;
    uint32_t pFLatency;
    if (TickPriority < (1UL << __NVIC_PRIO_BITS)) {
        HAL_NVIC_SetPriority(TIM4_IRQn, TickPriority, 0U);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
        uwTickPrio = TickPriority;
    } else
        return HAL_ERROR;
    __HAL_RCC_TIM4_CLK_ENABLE();
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;
    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    else
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
    htim4.Instance = TIM4;
    htim4.Init.Period = (1000000U / 1000U) - 1U;
    htim4.Init.Prescaler = uwPrescalerValue;
    htim4.Init.ClockDivision = 0;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    if (HAL_TIM_Base_Init(&htim4) == HAL_OK)
        return HAL_TIM_Base_Start_IT(&htim4);
    return HAL_ERROR;
}

void HAL_SuspendTick(void) {
    __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void) {
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
}

void MPU_Config(void) {
    MPU_Region_InitTypeDef MPU_InitStruct = {0};
    HAL_MPU_Disable();
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x0;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32B;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    while (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK);
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
        | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    while (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK);
    HAL_RCC_EnableCSS();

    Facus = HAL_RCC_GetSysClockFreq() / 1000000;
    unsigned int Reload = HAL_RCC_GetSysClockFreq() / 1000000;
    Reload *= 1000000 / configTICK_RATE_HZ;
    Facms = 1000 / configTICK_RATE_HZ;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = Reload;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
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
