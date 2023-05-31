#include "main.h"
#include "stm32h7xx_it.h"

extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern TIM_HandleTypeDef htim6;

extern EventGroupHandle_t interrupt_event;

void NMI_Handler(void) { SYSFAULT_HANDLE(); }
void HardFault_Handler(void) { error_handler(__FILE__, __LINE__); }
void MemManage_Handler(void) { error_handler(__FILE__, __LINE__); }
void BusFault_Handler(void) { error_handler(__FILE__, __LINE__); }
void UsageFault_Handler(void) { error_handler(__FILE__, __LINE__); }

void DMA1_Stream0_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_usart2_rx); }
void DMA1_Stream1_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_adc1); }
void DMA1_Stream2_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_usart1_tx); }
void DMA1_Stream3_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_usart1_rx); }
void DMA2_Stream0_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_spi3_tx); }

void SPI3_IRQHandler(void) { HAL_SPI_IRQHandler(&hspi3); }

void TIM6_DAC_IRQHandler(void) {
    if (hdac1.State != HAL_DAC_STATE_RESET)
        HAL_DAC_IRQHandler(&hdac1);
    HAL_TIM_IRQHandler(&htim6);
}

void USART1_IRQHandler(void) { HAL_UART_IRQHandler(&huart1); }
void USART2_IRQHandler(void) { HAL_UART_IRQHandler(&huart2); }

void MDMA_IRQHandler(void) { HAL_MDMA_IRQHandler(&hmdma_referee); }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6) {
        HAL_IncTick();
        if (pid_calculate_enable_flag == 1) {
            unsigned short dac_value = pid_calculate(power_info.charge_power);
            dac_set_output(dac_value);
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2 && interrupt_event != NULL) {
        BaseType_t higher_priority_task_woken = pdFALSE, result;
        result = xEventGroupSetBitsFromISR(interrupt_event, 0x04,
                                           &higher_priority_task_woken);
        if (result != pdFAIL)
            portYIELD_FROM_ISR(higher_priority_task_woken);
    } else if (huart->Instance == USART1 && interrupt_event != NULL) {
        BaseType_t higher_priority_task_woken = pdFALSE, result;
        result = xEventGroupSetBitsFromISR(interrupt_event, 0x08,
                                           &higher_priority_task_woken);
        if (result != pdFAIL)
            portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    __HAL_UNLOCK(huart);
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
        __HAL_UART_ENABLE_IT(huart, UART_IT_TC);
    }
}

__attribute__((section(".dma_ram")))volatile unsigned short adc_dma_result[6] = {0};
unsigned short adc_result_buffer[6][10] = {0}, adc_result[6] = {0}, buffer_counter = 0;
extern int compare_ushort(const void *a, const void *b);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    for (unsigned char counter = 0; counter < 6; counter++)
        adc_result_buffer[counter][buffer_counter] = adc_dma_result[counter];
    buffer_counter++;
    if (buffer_counter == 10) {
        unsigned long tmp_buffer[6] = {0, 0, 0, 0, 0, 0};
        buffer_counter = 0;
        for (unsigned char counter1 = 0; counter1 < 6; counter1++) {
            qsort((void *) adc_result_buffer[counter1], 10, sizeof(adc_result_buffer[0][0]), compare_ushort);
            for (unsigned char counter2 = 0; counter2 < 4; counter2++)
                tmp_buffer[counter1] += (unsigned long) adc_result_buffer[counter1][counter2 + 3];
            adc_result[counter1] = tmp_buffer[counter1] / 4;
        }
        update_powerinfo(adc_result);
    }
}

extern volatile unsigned char uart1_transmit_buffer[REFEREE_DMA_BUFFER_SIZE * 2];
extern const unsigned int k_power_manager_status_buffer_length;
void HAL_MDMA_BlockTransferCpltCallback(MDMA_HandleTypeDef *hmdma) {
    BaseType_t higher_priority_task_woken = pdFALSE, result;
    switch (mdma_status_flag) {
        case 0:HAL_UART_Transmit_DMA(&huart1, (unsigned char *) uart1_transmit_buffer, REFEREE_DMA_BUFFER_SIZE);
            break;
        case 1:
            result = xEventGroupSetBitsFromISR(interrupt_event, 0x01,
                                               &higher_priority_task_woken);
            if (result != pdFAIL)
                portYIELD_FROM_ISR(higher_priority_task_woken);
            break;
        case 2:
            result = xEventGroupSetBitsFromISR(interrupt_event, 0x02,
                                               &higher_priority_task_woken);
            if (result != pdFAIL)
                portYIELD_FROM_ISR(higher_priority_task_woken);
            break;
        case 3:
            HAL_UART_Transmit_DMA(&huart1,
                                  (unsigned char *) uart1_transmit_buffer,
                                  REFEREE_DMA_BUFFER_SIZE + k_power_manager_status_buffer_length);
            break;
        case 4:
            HAL_UART_Transmit_DMA(&huart1,
                                  (unsigned char *) uart1_transmit_buffer,
                                  k_power_manager_status_buffer_length);
            break;
        default:break;
    }
}
