#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "encrypt.h"
#include "system.h"
#include "config.h"
#include "tim.h"
#include "usart.h"

unsigned char TIM10_Flag = 0;
unsigned char TIM11_Flag = 0;

extern DMA_HandleTypeDef hdma_adc1;
extern CAN_HandleTypeDef hcan1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim4;
extern DAC_HandleTypeDef hdac;

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
    if (htim->Instance == TIM4)
        HAL_IncTick();
    else if (htim->Instance == TIM10) {
        TIM10_Flag = 1;
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
    } else if (htim->Instance == TIM11)
        TIM11_Flag = 1;
}

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

void DMA2_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_adc1);
}
void TIM1_UP_TIM10_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim10);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim11);
}
