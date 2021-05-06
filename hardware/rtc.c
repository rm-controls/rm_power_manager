//
// Created by Lao·Zhu on 2021/5/3.
//

#include "rtc.h"
#include "config.h"

RTC_HandleTypeDef hrtc;

HAL_StatusTypeDef RTC_Set_Time(unsigned char hour, unsigned char min, unsigned char sec) {
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_TimeStructure.Hours = hour;
    RTC_TimeStructure.Minutes = min;
    RTC_TimeStructure.Seconds = sec;
    RTC_TimeStructure.TimeFormat = RTC_HOURFORMAT12_AM;
    RTC_TimeStructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation = RTC_STOREOPERATION_RESET;
    return HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
}

//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7)
HAL_StatusTypeDef RTC_Set_Date(unsigned char year, unsigned char month, unsigned char date, unsigned char week) {
    RTC_DateTypeDef RTC_DateStructure;
    RTC_DateStructure.Date = date;
    RTC_DateStructure.Month = month;
    RTC_DateStructure.WeekDay = week;
    RTC_DateStructure.Year = year;
    return HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
}

void RTC_Config(void) {
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
    while (HAL_RTC_Init(&hrtc) != HAL_OK);

    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR31) != 0X5A5A) {
        RTC_Set_Time(5, 52, 0);
        RTC_Set_Date(21, 5, 3, 1);
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR31, 0X5A5A);
    }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle) {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (rtcHandle->Instance == RTC) {
        __HAL_RCC_RTC_CLK_ENABLE();
#if USE_OSC_32KHZ == 1
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
        while (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK);
#else
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
        while (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK);
#endif
        __HAL_RCC_RTC_ENABLE();
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle) {
    if (rtcHandle->Instance == RTC) {
        __HAL_RCC_RTC_DISABLE();
    }
}
