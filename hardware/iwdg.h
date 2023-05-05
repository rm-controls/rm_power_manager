#ifndef POWER_MANAGER_HARDWARE_IWDG_H_
#define POWER_MANAGER_HARDWARE_IWDG_H_

#include "stm32h7xx_hal.h"
#include "system.h"

extern IWDG_HandleTypeDef hiwdg1;

void iwdg_config(void);

#endif /* POWER_MANAGER_HARDWARE_IWDG_H_ */
