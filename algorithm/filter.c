//
// Created by LaoZhu on 2023/5/3.
//

#include "filter.h"

#define FILTER_SAMPLE_RATE  1000.0f
#define FILTER_CUTOFF_FREQ  200.0f

lowpass_filter_t chassis_power_filter, charge_power_filter, capacitor_voltage_filter;

void filter_config(void) {
    float RC = 1.0f / (2.0f * 3.14159265359f * FILTER_CUTOFF_FREQ);
    float alpha = 1.0f / (1.0f + RC * FILTER_SAMPLE_RATE);
    float beta = 1.0f - alpha;

    chassis_power_filter.prev_value = 0;
    chassis_power_filter.alpha = alpha;
    chassis_power_filter.beta = beta;
    charge_power_filter.prev_value = 0;
    charge_power_filter.alpha = alpha;
    charge_power_filter.beta = beta;
    capacitor_voltage_filter.prev_value = 0;
    capacitor_voltage_filter.alpha = alpha;
    capacitor_voltage_filter.beta = beta;
}

float get_filter_result(lowpass_filter_t *filter, float value) {
    float result = filter->alpha * value + filter->beta * filter->prev_value;
    filter->prev_value = result;
    return result;
}
