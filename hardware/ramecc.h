#ifndef POWER_MANAGER_HARDWARE_RAMECC_H_
#define POWER_MANAGER_HARDWARE_RAMECC_H_

#include "stm32h7xx_hal.h"
#include "system.h"

extern RAMECC_HandleTypeDef hramecc1_m1;
extern RAMECC_HandleTypeDef hramecc1_m2;
extern RAMECC_HandleTypeDef hramecc1_m3;
extern RAMECC_HandleTypeDef hramecc1_m4;
extern RAMECC_HandleTypeDef hramecc1_m5;
extern RAMECC_HandleTypeDef hramecc2_m1;
extern RAMECC_HandleTypeDef hramecc2_m2;
extern RAMECC_HandleTypeDef hramecc2_m3;
extern RAMECC_HandleTypeDef hramecc2_m4;
extern RAMECC_HandleTypeDef hramecc2_m5;
extern RAMECC_HandleTypeDef hramecc3_m1;
extern RAMECC_HandleTypeDef hramecc3_m2;

void ramecc_config(void);

#endif /* POWER_MANAGER_HARDWARE_RAMECC_H_ */

