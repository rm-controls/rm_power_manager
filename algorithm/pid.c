//
// Created by Lao·Zhu on 2023/5/12.
//

#include "pid.h"
#include "power.h"

static struct pid_calculate_t {
  float kp;
  float ki;
  float kd;
  float i_sum;
  float user;
  float last_error;
  float maximum;
  float minimum;
  float i_sum_max;
} charge_pid;

unsigned char pid_calculate_enable_flag = 0;

void pid_config(void) {
    charge_pid.kp = 1.0f;
    charge_pid.ki = 10.0f;
    charge_pid.kd = 7.5f;
    charge_pid.user = 0;
    charge_pid.i_sum_max = 185.0f;
    charge_pid.maximum = 2730;       //2V
    charge_pid.minimum = 1;
    pid_calculate_enable_flag = 1;
}

void pid_set_expect(float value) {
    charge_pid.user = value;
}

unsigned short pid_calculate(float collected) {
    if (charge_pid.user != 0) {
        if (charge_pid.user / power_info.capacitor_voltage > 10.0f)
            charge_pid.user = power_info.capacitor_voltage * 10.0f;
        float pid_error = charge_pid.user - collected, result;
        charge_pid.i_sum = charge_pid.i_sum + pid_error;
        charge_pid.i_sum = (charge_pid.i_sum > charge_pid.i_sum_max ? charge_pid.i_sum_max : charge_pid.i_sum);
        charge_pid.i_sum =
            (charge_pid.i_sum < (-1.0f * charge_pid.i_sum_max) ? (-1.0f * charge_pid.i_sum_max) : charge_pid.i_sum);
        result = charge_pid.kp * pid_error + charge_pid.ki * charge_pid.i_sum
            + charge_pid.kd * (pid_error - charge_pid.last_error);
        result = (result > charge_pid.maximum ? charge_pid.maximum : result);
        result = (result < charge_pid.minimum ? charge_pid.minimum : result);
        charge_pid.last_error = pid_error;
        return (unsigned short) result;
    } else {
        charge_pid.last_error = 0;
        charge_pid.i_sum = 0;
        return 0;
    }
}
