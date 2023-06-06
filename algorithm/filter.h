//
// Created by LaoZhu on 2023/5/3.
//

#ifndef POWER_MANAGER_ALGORITHM_FILTER_H_
#define POWER_MANAGER_ALGORITHM_FILTER_H_

typedef struct {
  float alpha;
  float beta;
  float prev_value;
} lowpass_filter_t;

extern lowpass_filter_t chassis_power_filter, charge_power_filter, capacitor_voltage_filter;

void filter_config(void);
float get_filter_result(lowpass_filter_t *filter, float value);

#endif  // POWER_MANAGER_ALGORITHM_FILTER_H_
