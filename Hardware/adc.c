#include "adc.h"
#include "stdlib.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

volatile unsigned short ADC_Result[5];
volatile unsigned short ADC_Buffer[5][10];
unsigned short Buf_Counter = 0;
volatile unsigned short ADC_FinalResult[5];

int compare_int(const void *a, const void *b) {
    return (*(int *) a) - (*(int *) b);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    for (unsigned char counter = 0; counter < 5; counter++)
        ADC_Buffer[counter][Buf_Counter] = ADC_Result[counter];
    Buf_Counter++;
    if (Buf_Counter == 10) {
        unsigned long Tmp_Buffer[5] = {0, 0, 0, 0, 0};
        Buf_Counter = 0;
        for (unsigned char counter = 0; counter < 5; counter++)
            qsort((void *) ADC_Buffer[counter], 10, sizeof(ADC_Buffer[0][0]), compare_int);
        for (unsigned char counter1 = 0; counter1 < 5; counter1++)
            for (unsigned char counter2 = 0; counter2 < 4; counter2++)
                Tmp_Buffer[counter1] += (unsigned long) ADC_Buffer[counter1][counter2 + 3];
        for (unsigned char counter = 0; counter < 5; counter++)
            ADC_FinalResult[counter] = Tmp_Buffer[counter] / 4;
    }
}

void ADC_Config(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ENABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 5;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    while (HAL_ADC_Init(&hadc1));
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig));
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 2;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig));
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = 3;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig));
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = 4;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig));
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = 5;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig));
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *) ADC_Result, 5);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (adcHandle->Instance == ADC1) {
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
            | GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        hdma_adc1.Instance = DMA2_Stream0;
        hdma_adc1.Init.Channel = DMA_CHANNEL_0;
        hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_adc1.Init.Mode = DMA_CIRCULAR;
        hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
        hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        while (HAL_DMA_Init(&hdma_adc1));
        __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle) {
    if (adcHandle->Instance == ADC1) {
        __HAL_RCC_ADC1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
            | GPIO_PIN_4);
        HAL_DMA_DeInit(adcHandle->DMA_Handle);
    }
}
