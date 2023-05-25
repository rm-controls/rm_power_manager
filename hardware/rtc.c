//
// Created by Lao·Zhu on 2021/5/3.
//

#include "rtc.h"
#include "system.h"
#include "referee.h"
#include "utils.h"
#include "version.h"

RTC_HandleTypeDef hrtc;

void rtc_set_time(unsigned char hour, unsigned char min, unsigned char sec) {
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_TimeStructure.Hours = hour;
    RTC_TimeStructure.Minutes = min;
    RTC_TimeStructure.Seconds = sec;
    RTC_TimeStructure.TimeFormat = RTC_HOURFORMAT12_AM;
    RTC_TimeStructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
}

void rtc_set_date(unsigned char year, unsigned char month, unsigned char date, unsigned char week) {
    RTC_DateTypeDef RTC_DateStructure;
    RTC_DateStructure.Date = date;
    RTC_DateStructure.Month = month;
    RTC_DateStructure.WeekDay = week;
    RTC_DateStructure.Year = year;
    HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
}

void rtc_config(void) {
    static datetime_t current_datetime = {0};

    referee_info.timestamp = 0;
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 99;
    hrtc.Init.SynchPrediv = 4999;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
        error_handler(__FILE__, __LINE__);
    HAL_PWR_EnableBkUpAccess();

    utc_second_to_date_time(CURRENT_TIME_UTC, &current_datetime);
    rtc_set_date(current_datetime.year - 2000, current_datetime.month, current_datetime.day, 1);
    rtc_set_time(current_datetime.hour, current_datetime.minute, current_datetime.second);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle) {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (rtcHandle->Instance == RTC) {
        HAL_PWREx_EnableBkUpReg();

        __HAL_RCC_RTC_CLK_ENABLE();
        __HAL_RCC_BKPRAM_CLK_ENABLE();

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV50;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            error_handler(__FILE__, __LINE__);

        __HAL_RCC_RTC_ENABLE();
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle) {
    if (rtcHandle->Instance == RTC) {
        __HAL_RCC_RTC_DISABLE();
    }
}
