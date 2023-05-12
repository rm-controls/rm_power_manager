//
// Created by Lao·Zhu on 2021/5/3.
//

#include "rtc.h"
#include "system.h"

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
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
        error_handler(__func__, __LINE__);

//    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR31) != 0X5A5A) {
//        RTC_Set_Time(18, 28, 5);
//        RTC_Set_Date(21, 5, 23, 7);
//        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR31, 0X5A5A);
//    }
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle) {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (rtcHandle->Instance == RTC) {
        __HAL_RCC_RTC_CLK_ENABLE();

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
        while (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK);

        __HAL_RCC_RTC_ENABLE();
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle) {
    if (rtcHandle->Instance == RTC) {
        __HAL_RCC_RTC_DISABLE();
    }
}
