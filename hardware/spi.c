//
// Created by Lao·Zhu on 2021/5/2.
//

#include "spi.h"
#include "watchdog.h"

SPI_HandleTypeDef hspi1;

unsigned char SPI_ReadWriteByte(unsigned char Txdata) {
    unsigned char Rxdata = 0;
    WRITE_REG(WWDG1->CR, (hwwdg1.Init.Counter));
    HAL_SPI_TransmitReceive(&hspi1, &Txdata, &Rxdata, 1, 0xFFFFFFFFUL);
    return Rxdata;
}

void SPI_SetSpeed(unsigned int SPI_BaudRatePrescaler) {
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    __HAL_SPI_DISABLE(&hspi1);
    hspi1.Instance->CR1 &= 0XFFC7;
    hspi1.Instance->CR1 |= SPI_BaudRatePrescaler;
    __HAL_SPI_ENABLE(&hspi1);
}

void SPI_Config(void) {
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 0x0;
    hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
    while (HAL_SPI_Init(&hspi1) != HAL_OK);
    __HAL_SPI_ENABLE(&hspi1);
    SPI_ReadWriteByte(0Xff);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (spiHandle->Instance == SPI1) {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI1;
        PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_CLKP;
        while (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK);

        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle) {
    if (spiHandle->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    }
}
