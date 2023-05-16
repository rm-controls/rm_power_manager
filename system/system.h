//
// Created by LaoZhu on 2023/4/9.
//

#ifndef POWER_MANAGER_SYSTEM_SYSTEM_H_
#define POWER_MANAGER_SYSTEM_SYSTEM_H_

#include "config.h"

#define SYSFAULT_HANDLE()   __asm volatile (\
                                            "TST LR, #4 \n"\
                                            "ITE EQ \n"\
                                            "MRSEQ R0, MSP \n"\
                                            "MRSNE R0, PSP \n"\
                                            "B sys_fault_handler \n")

typedef enum {
  power_on_reset = 1,
  rst_pin_reset = 2,
  software_reset = 3,
  iwdg_reset = 4,
  wwdg_reset = 5,
  lowpower_reset = 6,
  other_reset = 7,
} reset_reason_e;

void system_config(void);

void freertos_is_running(void);
void freertos_not_running(void);

void delayms(unsigned int xms);
void delayus(unsigned int xus);

void soft_reset(void);

void sys_fault_handler(unsigned int *hardfault_args);
void error_handler(const char *file, unsigned int line);
void error_check(void);

#endif //POWER_MANAGER_SYSTEM_SYSTEM_H_
