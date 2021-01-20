#include "main.h"
#include "stm32f4xx_it.h"

extern DMA_HandleTypeDef hdma_adc1;
extern CAN_HandleTypeDef hcan1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim4;

void NMI_Handler(void) {
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

void SVC_Handler(void) {

}

void DebugMon_Handler(void) {

}

void PendSV_Handler(void) {

}

void SysTick_Handler(void) {

}

void CAN1_RX0_IRQHandler(void) {
    HAL_CAN_IRQHandler(&hcan1);
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

void DMA2_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_adc1);
}
