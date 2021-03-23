#include "adc.h"
#include "stdlib.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
__attribute__((section(".adc_ram")))volatile unsigned short ADC_Result[5];
unsigned short ADC_Buffer[5][10], ADC_FinalResult[5], Buf_Counter = 0;

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
    ADC_MultiModeTypeDef multimode = {0};
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.NbrOfConversion = 5;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
    hadc1.Init.OversamplingMode = DISABLE;
    while (HAL_ADC_Init(&hadc1) != HAL_OK);
    while (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK);
    while ((HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED) != HAL_OK));
    multimode.Mode = ADC_MODE_INDEPENDENT;
    while (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK);
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_32CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    sConfig.OffsetSignedSaturation = DISABLE;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK);
    sConfig.Channel = ADC_CHANNEL_7;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK);
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_3;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK);
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = ADC_REGULAR_RANK_4;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK);
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_REGULAR_RANK_5;
    while (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *) &ADC_Result, 5);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (adcHandle->Instance == ADC1) {
        /**ADC1 GPIO Configuration
        PA6     ------> ADC1_INP3
        PA7     ------> ADC1_INP7
        PC4     ------> ADC1_INP4
        PC5     ------> ADC1_INP8
        PB0     ------> ADC1_INP9
        PB1     ------> ADC1_INP5
        */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        PeriphClkInitStruct.PLL2.PLL2M = 5;
        PeriphClkInitStruct.PLL2.PLL2N = 32;
        PeriphClkInitStruct.PLL2.PLL2P = 2;
        PeriphClkInitStruct.PLL2.PLL2Q = 2;
        PeriphClkInitStruct.PLL2.PLL2R = 2;
        PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
        PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
        PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
        PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
        while (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK);

        __HAL_RCC_ADC12_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        hdma_adc1.Instance = DMA1_Stream0;
        hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
        hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_adc1.Init.Mode = DMA_CIRCULAR;
        hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
        hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
        hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
        hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
        hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;
        while (HAL_DMA_Init(&hdma_adc1) != HAL_OK);

        __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);

        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle) {
    if (adcHandle->Instance == ADC1) {
        __HAL_RCC_ADC12_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6 | GPIO_PIN_7);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4 | GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);
        HAL_DMA_DeInit(adcHandle->DMA_Handle);
        HAL_NVIC_DisableIRQ(ADC_IRQn);
    }
}
