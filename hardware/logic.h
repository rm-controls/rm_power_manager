//
// Created by LaoZhu on 2023/4/10.
//

#ifndef POWER_MANAGER_HARDWARE_LOGIC_H_
#define POWER_MANAGER_HARDWARE_LOGIC_H_

void dac_set_output(unsigned short value);
void close_all_switches(void);
void charge_switch_only(void);

#endif //POWER_MANAGER_HARDWARE_LOGIC_H_
