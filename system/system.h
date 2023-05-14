//
// Created by LaoZhu on 2023/4/9.
//

#ifndef POWER_MANAGER_SYSTEM_SYSTEM_H_
#define POWER_MANAGER_SYSTEM_SYSTEM_H_

#include "config.h"

void system_config(void);

void freertos_is_running(void);
void freertos_not_running(void);

void delayms(unsigned int xms);
void delayus(unsigned int xus);

void error_handler(const char *file, unsigned int line);
void error_check(void);

#endif //POWER_MANAGER_SYSTEM_SYSTEM_H_
