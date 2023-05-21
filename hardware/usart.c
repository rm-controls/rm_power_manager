#include "usart.h"
#include "verify.h"
#include "string.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

extern volatile unsigned char uart2_receive_buffer1[128];

static unsigned char error_package_buffer[41] =
    {0xA5, 0x20, 0x00, 0x00, 0x00, 0x02, 0x83, [7 ... 40]=0x00};
void usart1_send_error_package(unsigned char error_code, unsigned char length, char *str) {
    error_package_buffer[1] = length;
    error_package_buffer[5] = error_code;
    error_package_buffer[4] = get_crc8_value(error_package_buffer, 4);
    memcpy(&error_package_buffer[7], str, length);
    unsigned short crc16_val = get_crc16_value((unsigned char *) error_package_buffer, 0x8300 | error_code,
                                               (unsigned char *) &error_package_buffer[7], 32);
    error_package_buffer[length + 7] = crc16_val & 0x00FF;
    error_package_buffer[length + 8] = (crc16_val >> 8) & 0x00FF;
    HAL_UART_Transmit(&huart1, error_package_buffer, length + 9, 0xFFFFFFFFUL);
}

void usart1_config(void) {
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
    if (HAL_UART_Init(&huart1) != HAL_OK)
        error_handler(__FILE__, __LINE__);
    if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        error_handler(__FILE__, __LINE__);
    if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        error_handler(__FILE__, __LINE__);
    if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
        error_handler(__FILE__, __LINE__);
}

void usart2_config(void) {
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
    if (HAL_UART_Init(&huart2) != HAL_OK)
        error_handler(__FILE__, __LINE__);
    if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        error_handler(__FILE__, __LINE__);
    if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        error_handler(__FILE__, __LINE__);
    if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
        error_handler(__FILE__, __LINE__);
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (uartHandle->Instance == USART1) {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
        PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            error_handler(__FILE__, __LINE__);

        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PB14     ------> USART1_TX
        PB15     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USART1_TX Init */
        hdma_usart1_tx.Instance = DMA1_Stream2;
        hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
        hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart1_tx.Init.Mode = DMA_NORMAL;
        hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
            error_handler(__FILE__, __LINE__);
        __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart1_tx);

        /* USART1_RX Init */
        hdma_usart1_rx.Instance = DMA1_Stream3;
        hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
        hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart1_rx.Init.Mode = DMA_NORMAL;
        hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
        hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
            error_handler(__FILE__, __LINE__);
        __HAL_LINKDMA(uartHandle, hdmarx, hdma_usart1_rx);

        HAL_NVIC_SetPriority(USART1_IRQn, UART1_GLOBAL_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    } else if (uartHandle->Instance == USART2) {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            error_handler(__FILE__, __LINE__);

        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        hdma_usart2_rx.Instance = DMA1_Stream0;
        hdma_usart2_rx.Init.Request = DMA_REQUEST_USART2_RX;
        hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart2_rx.Init.Mode = DMA_NORMAL;
        hdma_usart2_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
        hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
            error_handler(__FILE__, __LINE__);
        __HAL_LINKDMA(uartHandle, hdmarx, hdma_usart2_rx);

        HAL_NVIC_SetPriority(USART2_IRQn, UART2_GLOBAL_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {
    if (uartHandle->Instance == USART1) {
        __HAL_RCC_USART1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14 | GPIO_PIN_15);
        HAL_DMA_DeInit(uartHandle->hdmarx);
        HAL_DMA_DeInit(uartHandle->hdmatx);
    } else if (uartHandle->Instance == USART2) {
        __HAL_RCC_USART2_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}
