//
// Created by Lao·Zhu on 2021/1/21.
//

#include "main.h"
#include "stm32h7xx_it.h"

extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim4;

void NMI_Handler(void) {
    HAL_RCC_NMI_IRQHandler();
    DataSave_To_Flash(NMI_Reset);
    SoftReset();
    while (1);
}

void HardFault_Handler(void) {
    DataSave_To_Flash(Hardfault_Reset);
    SoftReset();
    while (1);
}

void MemManage_Handler(void) {
    while (1) {

    }
}

void BusFault_Handler(void) {
    DataSave_To_Flash(BusFault_Reset);
    SoftReset();
    while (1);
}

void UsageFault_Handler(void) {
    DataSave_To_Flash(UsageFault_Reset);
    SoftReset();
    while (1);
}

void DebugMon_Handler(void) {

}

void DMA1_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_adc1);
}

void ADC_IRQHandler(void) {
    HAL_ADC_IRQHandler(&hadc1);
}

void TIM4_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim4);
}

void TIM7_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim7);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4)
        HAL_IncTick();
    else if (htim->Instance == TIM7)
        Calculate_Power();
}

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

void USART2_IRQHandler(void) {
    if (__HAL_UART_GET_FLAG(&huart2, HAL_UART_ERROR_ORE) != RESET) {
        __HAL_UART_CLEAR_FLAG(&huart2, HAL_UART_ERROR_ORE);
    }
    HAL_UART_IRQHandler(&huart2);
}

void WWDG_IRQHandler(void) {
    HAL_WWDG_Refresh(&hwwdg1);
    HAL_WWDG_IRQHandler(&hwwdg1);
}
