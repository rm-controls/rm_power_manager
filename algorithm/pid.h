//
// Created by Lao·Zhu on 2023/5/12.
//

#ifndef POWER_MANAGER_ALGORITHM_PID_H_
#define POWER_MANAGER_ALGORITHM_PID_H_

extern unsigned char pid_calculate_enable_flag;

void pid_config(void);
void pid_set_expect(float value);
unsigned char pid_get_expect(void);
unsigned short pid_calculate(float collected);

#endif //POWER_MANAGER_ALGORITHM_PID_H_
