#include "crc.h"

CRC_HandleTypeDef hcrc;

void CRC_Config(void) {
    hcrc.Instance = CRC;
    hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
    hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
    hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
    hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
    hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
    while (HAL_CRC_Init(&hcrc) != HAL_OK);
}

void HAL_CRC_MspInit(CRC_HandleTypeDef *crcHandle) {

    if (crcHandle->Instance == CRC) {
        __HAL_RCC_CRC_CLK_ENABLE();
    }
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef *crcHandle) {

    if (crcHandle->Instance == CRC) {
        __HAL_RCC_CRC_CLK_DISABLE();
    }
}
