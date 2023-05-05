//
// Created by LaoZhu on 2023/5/3.
//

#ifndef POWER_MANAGER_ALGORITHM_FILTER_H_
#define POWER_MANAGER_ALGORITHM_FILTER_H_

#include "arm_math.h"

typedef struct {
  arm_fir_instance_f32 header;
  float filter_state[6];
  float result;
} fir_filter_t;

extern fir_filter_t chassis_power_filter, charge_power_filter, capacitor_voltage_filter;

void filter_config(void);
float get_filter_result(fir_filter_t *filter, float value);

#endif //POWER_MANAGER_ALGORITHM_FILTER_H_
