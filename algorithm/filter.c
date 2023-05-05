//
// Created by LaoZhu on 2023/5/3.
//

#include "filter.h"

// 4 order fir filter, 200Hz Fpass, 300Hz Fstop
const float filter_coefficient[5] = {-0.034569785f, 0.308319211f, 0.6030071378f,
                                     0.308319211f, -0.034569785f};
fir_filter_t chassis_power_filter, charge_power_filter, capacitor_voltage_filter;

void filter_config(void) {
    arm_fir_init_f32(&(chassis_power_filter.header), 5, filter_coefficient,
                     chassis_power_filter.filter_state, 1);
    arm_fir_init_f32(&(charge_power_filter.header), 5, filter_coefficient,
                     charge_power_filter.filter_state, 1);
    arm_fir_init_f32(&(capacitor_voltage_filter.header), 5, filter_coefficient,
                     capacitor_voltage_filter.filter_state, 1);
}

float get_filter_result(fir_filter_t *filter, float value) {
    arm_fir_f32(&(filter->header), &value, &filter->result, 1);
    return filter->result;
}
