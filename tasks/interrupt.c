//
// Created by LaoZhu on 2023/5/3.
//

#include "main.h"
#include "interrupt.h"

EventGroupHandle_t interrupt_event = NULL;

extern unsigned char *last_mdma_transfer_buf;
extern unsigned int last_mdma_transfer_pos;
extern volatile unsigned char uart1_transmit_buffer[UART_DMA_BUFFER_SIZE * 2];
extern volatile unsigned char power_manager_status_buffer[UART_DMA_BUFFER_SIZE];
extern const unsigned int k_power_manager_status_buffer_length;

__attribute__((section(".dma_ram")))volatile unsigned char uart2_receive_buffer1[UART_DMA_BUFFER_SIZE] = {0};
__attribute__((section(".dma_ram")))volatile unsigned char uart2_receive_buffer2[UART_DMA_BUFFER_SIZE] = {0};
static unsigned char uart2_receive_buffer = 1;

__attribute__((section(".dma_ram")))volatile unsigned char uart1_receive_buffer1[UART_DMA_BUFFER_SIZE] = {0};
__attribute__((section(".dma_ram")))volatile unsigned char uart1_receive_buffer2[UART_DMA_BUFFER_SIZE] = {0};
static unsigned char uart1_receive_buffer = 1;

void interrupt_handle_task(void *parameters) {
    (void) parameters;
    interrupt_event = xEventGroupCreate();
    HAL_UART_Receive_DMA(&huart2, (unsigned char *) uart2_receive_buffer1, UART_DMA_BUFFER_SIZE);
    HAL_UART_Receive_DMA(&huart1, (unsigned char *) uart1_receive_buffer1, UART_DMA_BUFFER_SIZE);
    while (1) {
        EventBits_t received_bits = xEventGroupWaitBits(interrupt_event, 0xFFFFFFUL,
                                                        pdTRUE, pdFALSE, portMAX_DELAY);
        if ((received_bits & 0x01) != 0) {
            HAL_MDMA_Start_IT(&hmdma_referee,
                              (unsigned int) power_manager_status_buffer,
                              (unsigned int) &uart1_transmit_buffer[last_mdma_transfer_pos],
                              k_power_manager_status_buffer_length,
                              1);
            mdma_status_flag = 2;
        } else if ((received_bits & 0x02) != 0) {
            HAL_MDMA_Start_IT(&hmdma_referee,
                              (unsigned int) &last_mdma_transfer_buf[last_mdma_transfer_pos],
                              (unsigned int) &uart1_transmit_buffer[last_mdma_transfer_pos
                                  + k_power_manager_status_buffer_length],
                              k_power_manager_status_buffer_length,
                              1);
            mdma_status_flag = 3;
        }

        if ((received_bits & 0x04) != 0) {
            switch (uart2_receive_buffer) {
                case 1:HAL_UART_Receive_DMA(&huart2, (unsigned char *) uart2_receive_buffer2, UART_DMA_BUFFER_SIZE);
                    referee_process_buffer((unsigned char *) uart2_receive_buffer1);
                    uart2_receive_buffer = 2;
                    break;
                default:
                case 2:HAL_UART_Receive_DMA(&huart2, (unsigned char *) uart2_receive_buffer1, UART_DMA_BUFFER_SIZE);
                    referee_process_buffer((unsigned char *) uart2_receive_buffer2);
                    uart2_receive_buffer = 1;
                    break;
            }
        }

        if ((received_bits & 0x08) != 0) {
            switch (uart1_receive_buffer) {
                case 1:HAL_UART_Receive_DMA(&huart1, (unsigned char *) uart1_receive_buffer2, UART_DMA_BUFFER_SIZE);
                    nuc_receive_callback((unsigned char *) uart1_receive_buffer1);
                    uart1_receive_buffer = 2;
                    break;
                default:
                case 2:HAL_UART_Receive_DMA(&huart1, (unsigned char *) uart1_receive_buffer1, UART_DMA_BUFFER_SIZE);
                    nuc_receive_callback((unsigned char *) uart1_receive_buffer2);
                    uart1_receive_buffer = 1;
                    break;
            }
        }

    }
}
