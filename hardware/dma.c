#include "dma.h"

MDMA_HandleTypeDef hmdma_referee;

extern void HAL_MDMA_BlockTransferCpltCallback(MDMA_HandleTypeDef *hmdma);

void dma_config(void) {

    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_MDMA_CLK_ENABLE();

    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, UART2_RX_DMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, ADC_DMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, UART1_TX_DMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, UART1_RX_DMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, SPI_DMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

    hmdma_referee.Instance = MDMA_Channel0;
    hmdma_referee.Init.Request = MDMA_REQUEST_SW;
    hmdma_referee.Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;
    hmdma_referee.Init.Priority = MDMA_PRIORITY_HIGH;
    hmdma_referee.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    hmdma_referee.Init.SourceInc = MDMA_SRC_INC_BYTE;
    hmdma_referee.Init.DestinationInc = MDMA_DEST_INC_BYTE;
    hmdma_referee.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
    hmdma_referee.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
    hmdma_referee.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
    hmdma_referee.Init.SourceBurst = MDMA_SOURCE_BURST_16BEATS;
    hmdma_referee.Init.DestBurst = MDMA_DEST_BURST_16BEATS;
    hmdma_referee.Init.BufferTransferLength = REFEREE_DMA_BUFFER_SIZE;
    hmdma_referee.Init.SourceBlockAddressOffset = 0;
    hmdma_referee.Init.DestBlockAddressOffset = 0;
    if (HAL_MDMA_Init(&hmdma_referee) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    HAL_MDMA_RegisterCallback(&hmdma_referee, HAL_MDMA_XFER_BLOCKCPLT_CB_ID, HAL_MDMA_BlockTransferCpltCallback);

    HAL_NVIC_SetPriority(MDMA_IRQn, MDMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(MDMA_IRQn);
}

__attribute__((section(".dma_ram")))volatile unsigned char uart1_transmit_buffer[REFEREE_DMA_BUFFER_SIZE * 2] = {0};
unsigned char power_manager_status_send_flag = 0, mdma_status_flag = 0, *last_mdma_transfer_buf;
unsigned int last_mdma_transfer_pos = 0;
extern const unsigned int k_power_manager_status_buffer_length;
void mdma_transmit_buffer(const unsigned char *source, int length) {
    if (length != -1 && power_manager_status_send_flag == 1) {
        last_mdma_transfer_buf = (unsigned char *) source;
        last_mdma_transfer_pos = length;
        mdma_status_flag = 1;
        hmdma_referee.State = HAL_MDMA_STATE_READY;
        hmdma_referee.Lock = HAL_UNLOCKED;
        HAL_MDMA_Start_IT(&hmdma_referee,
                          (unsigned int) source,
                          (unsigned int) uart1_transmit_buffer,
                          length,
                          1);
    } else if (power_manager_status_send_flag == 0) {
        mdma_status_flag = 0;
        hmdma_referee.State = HAL_MDMA_STATE_READY;
        hmdma_referee.Lock = HAL_UNLOCKED;
        HAL_MDMA_Start_IT(&hmdma_referee,
                          (unsigned int) source,
                          (unsigned int) uart1_transmit_buffer,
                          REFEREE_DMA_BUFFER_SIZE,
                          1);
    } else if (power_manager_status_send_flag == 2) {
        mdma_status_flag = 4;
        hmdma_referee.State = HAL_MDMA_STATE_READY;
        hmdma_referee.Lock = HAL_UNLOCKED;
        HAL_MDMA_Start_IT(&hmdma_referee,
                          (unsigned int) source,
                          (unsigned int) uart1_transmit_buffer,
                          k_power_manager_status_buffer_length,
                          1);
    }
    power_manager_status_send_flag = 0;
}
