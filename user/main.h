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

/////////////// Hardware ///////////////
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "timer.h"

/////////////// Device ///////////////
#include "pid.h"
#include "decrypt.h"
#include "encrypt.h"
#include "filter.h"
#include "refree.h"

/////////////// Functions ///////////////
#include "calculate.h"
#include "report.h"
#include "fsm.h"
#include "protect.h"

#endif /* __MAIN_H */
