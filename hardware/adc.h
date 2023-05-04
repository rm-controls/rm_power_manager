#ifndef POWER_MANAGER_HARDWARE_ADC_H_
#define POWER_MANAGER_HARDWARE_ADC_H_

#include "stm32h7xx_hal.h"
#include "system.h"

extern ADC_HandleTypeDef hadc1;

void adc_config(void);

#endif /* POWER_MANAGER_HARDWARE_ADC_H_ */

