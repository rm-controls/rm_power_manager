#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f4xx_hal.h"

extern volatile unsigned short ADC_Result[5];

extern ADC_HandleTypeDef hadc1;
void ADC_Config(void);

#endif /* __ADC_H__ */
