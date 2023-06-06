#ifndef POWER_MANAGER_USER_MAIN_H_
#define POWER_MANAGER_USER_MAIN_H_

/////////////////////////// System ///////////////////////////
#include "stm32h7xx_hal.h"
#include "system.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "utils.h"
#include "version.h"

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
#include "key.h"
#include "iic.h"

/////////////////////////// GUI ///////////////////////////
#include "lcd.h"
#include "gui_base.h"
#include "gui_button.h"
#include "gui_chart.h"
#include "gui_lable.h"
#include "gui_numeric.h"
#include "gui_listbox.h"
#include "gui_textbox.h"

/////////////////////////// Algorithm ///////////////////////////
#include "power.h"
#include "referee.h"
#include "filter.h"
#include "verify.h"
#include "pid.h"
#include "nuc_api.h"
#include "selfcheck.h"

/////////////////////////// Tasks ///////////////////////////
#include "interrupt.h"
#include "fsm.h"
#include "protect.h"
#include "gui.h"

/////////////////////////// Forms ///////////////////////////
#include "form_logic.h"
#include "main_form.h"
#include "log_form.h"
#include "settings_form.h"
#include "osinfo_form.h"
#include "sysinfo_form.h"
#include "logview_form.h"
#include "selfcheck_form.h"

#endif  // POWER_MANAGER_USER_MAIN_H_
