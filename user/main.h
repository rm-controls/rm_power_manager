#ifndef POWER_MANAGER_SYSTEM_USER_MAIN_H_
#define POWER_MANAGER_SYSTEM_USER_MAIN_H_

/////////////////////////// System ///////////////////////////
#include "stm32h7xx_hal.h"
#include "system.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "utils.h"

/////////////////////////// FreeRTOS ///////////////////////////
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/////////////////////////// Hardware ///////////////////////////
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "iwdg.h"
#include "ramecc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "logic.h"
#include "rtc.h"

/////////////////////////// GUI ///////////////////////////
#include "lcd.h"
#include "gui_base.h"

/////////////////////////// Algorithm ///////////////////////////
#include "power.h"
#include "referee.h"
#include "filter.h"
#include "verify.h"
#include "pid.h"

/////////////////////////// Tasks ///////////////////////////
#include "interrupt.h"
#include "fsm.h"
#include "protect.h"
#include "gui.h"

#endif /* POWER_MANAGER_SYSTEM_USER_MAIN_H_ */
