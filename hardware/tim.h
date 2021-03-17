#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;

void TIM_Config(void);

#endif /* __TIM_H__ */
