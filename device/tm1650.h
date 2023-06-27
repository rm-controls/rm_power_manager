//
// Created by LaoZhu on 2023/6/6.
//

#ifndef POWER_MANAGER_DEVICE_TM1650_H_
#define POWER_MANAGER_DEVICE_TM1650_H_

void tm1650_config(void);
void tm1650_write_data(unsigned char disp_on, const unsigned char *data);

#endif //POWER_MANAGER_DEVICE_TM1650_H_
