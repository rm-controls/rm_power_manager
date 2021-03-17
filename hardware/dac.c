#include "dac.h"

DAC_HandleTypeDef hdac;

void DAC_Config(void) {
    DAC_ChannelConfTypeDef sConfig = {0};
    hdac.Instance = DAC;
    while (HAL_DAC_Init(&hdac));
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
//    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    while (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2));
    HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0X0000);
}

void HAL_DAC_MspInit(DAC_HandleTypeDef *dacHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (dacHandle->Instance == DAC) {
        __HAL_RCC_DAC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef *dacHandle) {
    if (dacHandle->Instance == DAC) {
        __HAL_RCC_DAC_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
    }
}
