#ifndef __MAIN_H
#define __MAIN_H

/////////////// System ///////////////
#include "stm32h7xx_hal.h"
#include "port.h"
#include "system.h"
#include "config.h"
#include "usertask.h"
#include "string.h"

/////////////// FreeRTOS ///////////////
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/////////////// Hardware ///////////////
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "timer.h"
#include "watchdog.h"
#include "key.h"
#include "lcd.h"

/////////////// Device ///////////////
#include "pid.h"
#include "decrypt.h"
#include "encrypt.h"
#include "filter.h"
#include "refree.h"
#include "gui.h"
#include "color.h"

/////////////// Functions ///////////////
#include "calculate.h"
#include "report.h"
#include "fsm.h"
#include "protect.h"
#include "datasave.h"

#endif /* __MAIN_H */
