#include "dac.h"

DAC_HandleTypeDef hdac1;

void dac_config(void) {
    DAC_ChannelConfTypeDef sConfig = {0};

    hdac1.Instance = DAC1;
    if (HAL_DAC_Init(&hdac1) != HAL_OK) {
        error_handler(__FILE__, __LINE__);;
    }

    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
    if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
        error_handler(__FILE__, __LINE__);
    }
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x0000UL);
}

void HAL_DAC_MspInit(DAC_HandleTypeDef *dacHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (dacHandle->Instance == DAC1) {
        __HAL_RCC_DAC12_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**DAC1 GPIO Configuration
        PA4     ------> DAC1_OUT1
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* DAC1 interrupt Init */
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef *dacHandle) {
    if (dacHandle->Instance == DAC1) {
        __HAL_RCC_DAC12_CLK_DISABLE();
        /**DAC1 GPIO Configuration
        PA4     ------> DAC1_OUT1
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
    }
}
