#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "fsm.h"
#include "sysinfo_form.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

extern QueueHandle_t Referee_Data;
unsigned char aRxBuffer1[1] = {0};
unsigned char aRxBuffer2[64] = {0};
unsigned char ModeBuffer = 0;
unsigned char UART1_IT_Flag = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        if (Referee_Data != NULL) {
            BaseType_t pxHigherPriorityTaskWoken;
            xQueueSendToBackFromISR(Referee_Data, aRxBuffer2, &pxHigherPriorityTaskWoken);
        }
        HAL_UART_Receive_IT(&huart2, aRxBuffer2, 64);
    } else {
        if (aRxBuffer1[0] == 0x01)
            ModeBuffer = 1;
        else {
            switch (ModeBuffer) {
                case 1:
                    if (aRxBuffer1[0] == 0x00)
                        ModeBuffer = 2;
                    else
                        ModeBuffer = 0;
                    break;
                case 2:
                    if (aRxBuffer1[0] == 0x00)
                        ModeBuffer = 3;
                    else
                        ModeBuffer = 0;
                    break;
                case 3:
                    if (aRxBuffer1[0] == 0x39 || aRxBuffer1[0] == 0x3A)
                        ModeBuffer = 4;
                    else
                        ModeBuffer = 0;
                    break;
                case 4:
                    if (aRxBuffer1[0] == 0xE0)
                        Setting_FSM_Mode = Normal_Optimized;
                    else if (aRxBuffer1[0] == 0xC8)
                        Setting_FSM_Mode = ChargeFirst_Optimized;
                    else if (aRxBuffer1[0] == 0xCC)
                        Setting_FSM_Mode = UseFirst_Optimized;
                    else if (aRxBuffer1[0] == 0xD4)
                        Setting_FSM_Mode = SucapTest_Optimized;
                    ModeBuffer = 0;
                    break;
                default:ModeBuffer = 0;
                    break;
            }
        }
        UART1_IT_Flag = HAL_UART_Receive_IT(&huart1, aRxBuffer1, 1);
    }
}

void UART1_Config(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    while (HAL_UART_Init(&huart1) != HAL_OK);
    while (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK);
    while (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK);
    while (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart1, aRxBuffer1, 1);
}

void UART2_Config(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    while (HAL_UART_Init(&huart2) != HAL_OK);
    while (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK);
    while (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK);
    while (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart2, aRxBuffer2, 64);
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (uartHandle->Instance == USART1) {

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
        PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
        while (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK);

        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PB14     ------> USART1_TX
        PB15     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    } else if (uartHandle->Instance == USART2) {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        while (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK);
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {
    if (uartHandle->Instance == USART1) {
        __HAL_RCC_USART1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14 | GPIO_PIN_15);
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    } else if (uartHandle->Instance == USART2) {
        __HAL_RCC_USART2_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}
