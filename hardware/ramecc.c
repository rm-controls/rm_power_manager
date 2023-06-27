#include "ramecc.h"

RAMECC_HandleTypeDef hramecc1_m1;
RAMECC_HandleTypeDef hramecc1_m2;
RAMECC_HandleTypeDef hramecc1_m3;
RAMECC_HandleTypeDef hramecc1_m4;
RAMECC_HandleTypeDef hramecc1_m5;
RAMECC_HandleTypeDef hramecc2_m1;
RAMECC_HandleTypeDef hramecc2_m2;
RAMECC_HandleTypeDef hramecc2_m3;
RAMECC_HandleTypeDef hramecc2_m4;
RAMECC_HandleTypeDef hramecc2_m5;
RAMECC_HandleTypeDef hramecc3_m1;
RAMECC_HandleTypeDef hramecc3_m2;

void ramecc_config(void) {

    hramecc1_m1.Instance = RAMECC1_Monitor1;
    if (HAL_RAMECC_Init(&hramecc1_m1) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc1_m2.Instance = RAMECC1_Monitor2;
    if (HAL_RAMECC_Init(&hramecc1_m2) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc1_m3.Instance = RAMECC1_Monitor3;
    if (HAL_RAMECC_Init(&hramecc1_m3) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc1_m4.Instance = RAMECC1_Monitor4;
    if (HAL_RAMECC_Init(&hramecc1_m4) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc1_m5.Instance = RAMECC1_Monitor5;
    if (HAL_RAMECC_Init(&hramecc1_m5) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc2_m1.Instance = RAMECC2_Monitor1;
    if (HAL_RAMECC_Init(&hramecc2_m1) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc2_m2.Instance = RAMECC2_Monitor2;
    if (HAL_RAMECC_Init(&hramecc2_m2) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc2_m3.Instance = RAMECC2_Monitor3;
    if (HAL_RAMECC_Init(&hramecc2_m3) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc2_m4.Instance = RAMECC2_Monitor4;
    if (HAL_RAMECC_Init(&hramecc2_m4) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc2_m5.Instance = RAMECC2_Monitor5;
    if (HAL_RAMECC_Init(&hramecc2_m5) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc3_m1.Instance = RAMECC3_Monitor1;
    if (HAL_RAMECC_Init(&hramecc3_m1) != HAL_OK)
        error_handler(__FILE__, __LINE__);

    hramecc3_m2.Instance = RAMECC3_Monitor2;
    if (HAL_RAMECC_Init(&hramecc3_m2) != HAL_OK)
        error_handler(__FILE__, __LINE__);
}
