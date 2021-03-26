#include "main.h"
#include "stm32h7xx_it.h"

extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim4;

void NMI_Handler(void) {
    HAL_RCC_NMI_IRQHandler();
    while (1) {
    }
}

void HardFault_Handler(void) {
    while (1) {
    }
}

void MemManage_Handler(void) {
    while (1) {

    }
}

void BusFault_Handler(void) {
    while (1) {

    }
}
void UsageFault_Handler(void) {
    while (1) {

    }
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) {
        HAL_IncTick();
    }
}

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}
