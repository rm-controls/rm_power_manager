#include "iwdg.h"

IWDG_HandleTypeDef hiwdg1;

void iwdg_config(void) {
    hiwdg1.Instance = IWDG1;
    hiwdg1.Init.Prescaler = IWDG_PRESCALER_4;
    hiwdg1.Init.Window = 4095;
    hiwdg1.Init.Reload = 4095;
    if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
        error_handler(__FILE__, __LINE__);
}
