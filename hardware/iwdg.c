#include "iwdg.h"

IWDG_HandleTypeDef hiwdg1;

void iwdg_config(void) {        // 500ms
    hiwdg1.Instance = IWDG1;
    hiwdg1.Init.Prescaler = IWDG_PRESCALER_4;
    hiwdg1.Init.Window = IWDG_WINDOW_DISABLE;
    hiwdg1.Init.Reload = 0x0FFF;
    if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
        error_handler(__FILE__, __LINE__);
}
