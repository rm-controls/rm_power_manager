//
// Created by LaoZhu on 2023/4/10.
//

#ifndef POWER_MANAGER_HARDWARE_LOGIC_H_
#define POWER_MANAGER_HARDWARE_LOGIC_H_

void dac_set_output(unsigned short value);
void close_all_switches(void);
void charge_switch_only(void);
void passthrough_switch_only(unsigned char is_last_boost);
void charge_with_boost_switches(unsigned char is_last_pass);

#endif //POWER_MANAGER_HARDWARE_LOGIC_H_
