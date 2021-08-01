//
// Created by Lao·Zhu on 2021/1/21.
//

#include "dma.h"

void DMA_Config(void) {
    __HAL_RCC_DMA1_CLK_ENABLE();

    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}
