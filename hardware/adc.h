#ifndef __ADC_H__
#define __ADC_H__

#include "stm32h7xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern unsigned short ADC_FinalResult[6];
void ADC_Config(void);

#endif /* __ADC_H__ */
