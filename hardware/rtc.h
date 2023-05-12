//
// Created by Lao·Zhu on 2021/5/3.
//

#ifndef PROGRAM_HARDWARE_RTC_H_
#define PROGRAM_HARDWARE_RTC_H_

#include "stm32h7xx_hal.h"

extern RTC_HandleTypeDef hrtc;

void rtc_config(void);
void rtc_set_date(unsigned char year, unsigned char month, unsigned char date, unsigned char week);
void rtc_set_time(unsigned char hour, unsigned char min, unsigned char sec);

#endif //PROGRAM_HARDWARE_RTC_H_
