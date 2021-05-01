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
#include "iic.h"

/////////////// Device ///////////////
#include "pid.h"
#include "decrypt.h"
#include "encrypt.h"
#include "filter.h"
#include "refree.h"
#include "eeprom.h"

/////////////// Functions ///////////////
#include "calculate.h"
#include "report.h"
#include "fsm.h"
#include "protect.h"
#include "datasave.h"

/////////////// GUI ///////////////
#include "gui_base.h"
#include "color.h"
#include "gui_button.h"
#include "gui_chart.h"
#include "gui_lable.h"

/////////////// Forms ///////////////
#include "form_logic.h"
#include "main_form.h"
#include "log_form.h"
#include "settings_form.h"

/////////////// FatFS ///////////////
#include "ff.h"

#endif /* __MAIN_H */
