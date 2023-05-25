//
// Created by LaoZhu on 2023/4/10.
//

#ifndef POWER_MANAGER_ALGORITHM_POWER_H_
#define POWER_MANAGER_ALGORITHM_POWER_H_

typedef struct {
  float chassis_current;
  float charge_current;
  float battery_voltage;
  float chassis_voltage;
  float capacitor_voltage;
  float chassis_power;
  float charge_power;
  float capacitor_percent;
  float expect_chassis_power;
} power_info_t;

typedef struct {
  unsigned short chassis_current_offset;
  unsigned short charge_current_offset;
  float current_k;
  float current_b;
} calibrate_params_t;

extern volatile calibrate_params_t calibrate_params;
extern volatile power_info_t power_info;

void pack_powerinfo_buffer();
void update_powerinfo(const unsigned short *adc_result);
void calibrate_params_config(void);
void calibrate_referee_config(void);

#endif //POWER_MANAGER_ALGORITHM_POWER_H_
