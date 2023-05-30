//
// Created by LaoZhu on 2023/4/9.
//

#include "system.h"
#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtc.h"
#include "usart.h"
#include "string.h"

TIM_HandleTypeDef htim6;
static unsigned int factor_us = 0;
unsigned int freertos_running_flag = 0;

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler;
    uint32_t uwPrescalerValue;
    uint32_t pFLatency;

    TickPriority = TIM6_GLOBAL_PRIORITY;
    if (TickPriority < (1UL << __NVIC_PRIO_BITS)) {
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority, 0U);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        uwTickPrio = TickPriority;
    } else
        return HAL_ERROR;

    __HAL_RCC_TIM6_CLK_ENABLE();

    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    else
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();

    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
    htim6.Instance = TIM6;
    htim6.Init.Period = (1000000U / 1000U) - 1U;
    htim6.Init.Prescaler = uwPrescalerValue;
    htim6.Init.ClockDivision = 0;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&htim6) == HAL_OK)
        return HAL_TIM_Base_Start_IT(&htim6);

    return HAL_ERROR;
}

void HAL_SuspendTick(void) {
    __HAL_TIM_DISABLE_IT(&htim6, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void) {
    __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE);
}

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

void mpu_config(void) {
    MPU_Region_InitTypeDef MPU_InitStruct = {0};
    HAL_MPU_Disable();

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x0;
    MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x24040000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void system_config(void) {

    mpu_config();
    SCB_EnableICache();
    SCB_EnableDCache();
    HAL_Init();

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        error_handler(__FILE__, __LINE__);
    }

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

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        error_handler(__FILE__, __LINE__);
    }

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_SPI3;
    PeriphClkInitStruct.PLL2.PLL2M = 5;
    PeriphClkInitStruct.PLL2.PLL2N = 32;
    PeriphClkInitStruct.PLL2.PLL2P = 2;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
    PeriphClkInitStruct.PLL2.PLL2R = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        error_handler(__FILE__, __LINE__);
    }

    __HAL_RCC_D2SRAM1_CLK_ENABLE();
    __HAL_RCC_D2SRAM2_CLK_ENABLE();
    __HAL_RCC_D2SRAM3_CLK_ENABLE();

    factor_us = HAL_RCC_GetSysClockFreq() / 1000000;
    unsigned int Reload = HAL_RCC_GetSysClockFreq() / 1000000;
    Reload *= 1000000 / 1000;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = Reload;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    freertos_running_flag = 0;
}

void freertos_is_running(void) {
    freertos_running_flag = 1;
}

void freertos_not_running(void) {
    freertos_running_flag = 0;
}

void delayus(unsigned int xus) {
    unsigned int Ticks, Time_Old, Time_Now, Time_Count = 0;
    unsigned int Reload = SysTick->LOAD;
    Ticks = xus * factor_us;
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

void delayms(unsigned int xms) {
    if (freertos_running_flag == 0)
        HAL_Delay(xms);
    else
        vTaskDelay(xms);
}

static reset_reason_e get_reset_reason(void) {
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET) {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return power_on_reset;
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET) {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return rst_pin_reset;
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET) {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return software_reset;
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST) != RESET) {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return iwdg_reset;
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDG1RST) != RESET) {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return wwdg_reset;
    } else if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWR1RST) != RESET
        || __HAL_RCC_GET_FLAG(RCC_FLAG_LPWR2RST) != RESET) {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return lowpower_reset;
    } else {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        return other_reset;
    }
}

void soft_reset(void) {
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void) xTask;
    __disable_irq();
    HAL_PWR_EnableBkUpAccess();
    for (unsigned char counter = 0; counter < 32; counter++)
        HAL_RTCEx_BKUPWrite(&hrtc, counter, 0x00000000);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0X5A5A0003);
    for (unsigned int counter = 1; counter < (strlen(pcTaskName) + 1); counter++)
        HAL_RTCEx_BKUPWrite(&hrtc, counter, *pcTaskName++);
    soft_reset();
}

void sys_fault_handler(const unsigned int *hardfault_args) {
    unsigned int stacked_r0 = ((unsigned long) hardfault_args[0]);
    unsigned int stacked_r1 = ((unsigned long) hardfault_args[1]);
    unsigned int stacked_r2 = ((unsigned long) hardfault_args[2]);
    unsigned int stacked_r3 = ((unsigned long) hardfault_args[3]);
    unsigned int stacked_r12 = ((unsigned long) hardfault_args[4]);
    unsigned int stacked_lr = ((unsigned long) hardfault_args[5]);
    unsigned int stacked_pc = ((unsigned long) hardfault_args[6]);
    unsigned int stacked_psr = ((unsigned long) hardfault_args[7]);
    __disable_irq();
    HAL_PWR_EnableBkUpAccess();
    for (unsigned char counter = 0; counter < 32; counter++)
        HAL_RTCEx_BKUPWrite(&hrtc, counter, 0x00000000);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0X5A5A0002);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, stacked_r0);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, stacked_r1);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, stacked_r2);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, stacked_r3);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, stacked_r12);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR6, stacked_lr);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR7, stacked_pc);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR8, stacked_psr);
    soft_reset();
}

void error_handler(const char *file, unsigned int line) {
    unsigned char copy_counter = 2;
    __disable_irq();
    HAL_PWR_EnableBkUpAccess();
    for (unsigned char counter = 0; counter < 32; counter++)
        HAL_RTCEx_BKUPWrite(&hrtc, counter, 0x00000000);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0X5A5A0001);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, line);
    char *last_space = strrchr(file, '\\');
    if (last_space == NULL) {
        last_space = strrchr(file, '/');
        if (last_space == NULL)
            last_space = (char *) file;
    }
    last_space++;
    while (*last_space != '\0') {
        HAL_RTCEx_BKUPWrite(&hrtc, copy_counter, *last_space++);
        copy_counter++;
        if (copy_counter >= 31)
            break;
    }
    soft_reset();
}

static char error_info_str[32] = {0};
void error_check(void) {
    reset_reason_e reset_reason = get_reset_reason();
    HAL_PWR_EnableBkUpAccess();
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == 0X5A5A0001) {
        for (unsigned char counter = 2; counter < 32; counter++)
            error_info_str[counter - 1] = HAL_RTCEx_BKUPRead(&hrtc, counter) & 0x000000FF;
        error_info_str[0] = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) % 0x000000FF;
        usart1_send_error_package(0x02, 32, error_info_str);
    } else if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == 0X5A5A0002) {
        for (unsigned char counter = 1; counter < 9; counter++) {
            unsigned int current_reg = HAL_RTCEx_BKUPRead(&hrtc, counter);
            error_info_str[(counter - 1) * 4] = ((current_reg >> 24) & 0x000000FF);
            error_info_str[(counter - 1) * 4 + 1] = ((current_reg >> 16) & 0x000000FF);
            error_info_str[(counter - 1) * 4 + 2] = ((current_reg >> 8) & 0x000000FF);
            error_info_str[(counter - 1) * 4 + 3] = (current_reg & 0x000000FF);
            usart1_send_error_package(0x03, 32, error_info_str);
        }
    } else if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == 0X5A5A0003) {
        for (unsigned char counter = 1; counter < 32; counter++)
            error_info_str[counter - 1] = HAL_RTCEx_BKUPRead(&hrtc, counter) & 0x000000FF;
        usart1_send_error_package(0x04, 32, error_info_str);
    } else {
        if (reset_reason != power_on_reset && reset_reason != rst_pin_reset &&
            HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0X83838383) {
            error_info_str[0] = reset_reason;
            error_info_str[1] = 0;
            usart1_send_error_package(0x05, 2, error_info_str);
        }
    }
    if ((reset_reason == power_on_reset || reset_reason == rst_pin_reset)
        && HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == 0X83838383)
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0X83838384);
}
