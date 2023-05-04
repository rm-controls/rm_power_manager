#ifndef POWER_MANAGER_HARDWARE_DAC_H_
#define POWER_MANAGER_HARDWARE_DAC_H_

#include "stm32h7xx_hal.h"
#include "system.h"

extern DAC_HandleTypeDef hdac1;

void dac_config(void);

#endif /* POWER_MANAGER_HARDWARE_DAC_H_ */

