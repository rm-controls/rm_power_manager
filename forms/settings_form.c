//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

Button_Struct_t TurnBack_Button2;
Numeric_Struct_t RTCYear_Numeric, RTCMonth_Numeric, RTCDate_Numeric;
Numeric_Struct_t RTCHour_Numeric, RTCMinute_Numeric, RTCSecond_Numeric;
Lable_Struct_t RTCDate_Lable, RTCTime_Lable;

void TurnBack_Button2_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void RTCYear_Numeric_Callback(void *object, unsigned char key) {

}

void SettingsForm_Update(void) {

}

void SettingsForm_Init(void) {
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    char String_Buffer_Tmp[16];
    HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BIN);
    GUI_Clear(C_WHITE);

    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button2;
    TurnBack_Button2.X_Pos = 24;
    TurnBack_Button2.Y_Pos = 135;
    TurnBack_Button2.Width = 80;
    TurnBack_Button2.Height = 20;
    TurnBack_Button2.Text = "Turn Back";
    TurnBack_Button2.NextButton = &RTCYear_Numeric;
    TurnBack_Button2.CallbackFunction = TurnBack_Button2_Callback;

    RTCDate_Lable.X_Pos = 2;
    RTCDate_Lable.Y_Pos = 8;
    RTCDate_Lable.Color = C_BLACK;
    RTCDate_Lable.Text = "Date";

    memset(String_Buffer_Tmp, 0x00, sizeof(String_Buffer_Tmp));
    sprintf(String_Buffer_Tmp, "20%02d", RTC_DateStruct.Year);
    RTCYear_Numeric.X_Pos = 26;
    RTCYear_Numeric.Y_Pos = 4;
    RTCYear_Numeric.Width = 38;
    RTCYear_Numeric.Height = 20;
    RTCYear_Numeric.Value = RTC_DateStruct.Year;
    RTCYear_Numeric.Text = (const char *) String_Buffer_Tmp;
    RTCYear_Numeric.Widget_Type = Numeric_Widget_Type;
    RTCYear_Numeric.NextNumeric = &RTCMonth_Numeric;
    RTCYear_Numeric.CallbackFunction = RTCYear_Numeric_Callback;
    GUI_InitNumeric(&RTCYear_Numeric);

    memset(String_Buffer_Tmp, 0x00, sizeof(String_Buffer_Tmp));
    sprintf(String_Buffer_Tmp, "%02d", RTC_DateStruct.Month);
    RTCMonth_Numeric.X_Pos = 66;
    RTCMonth_Numeric.Y_Pos = 4;
    RTCMonth_Numeric.Width = 28;
    RTCMonth_Numeric.Height = 20;
    RTCMonth_Numeric.Value = RTC_DateStruct.Month;
    RTCMonth_Numeric.Text = (const char *) String_Buffer_Tmp;
    RTCMonth_Numeric.Widget_Type = Numeric_Widget_Type;
    RTCMonth_Numeric.NextNumeric = &RTCDate_Numeric;
    RTCMonth_Numeric.CallbackFunction = RTCYear_Numeric_Callback;
    GUI_InitNumeric(&RTCMonth_Numeric);

    memset(String_Buffer_Tmp, 0x00, sizeof(String_Buffer_Tmp));
    sprintf(String_Buffer_Tmp, "%02d", RTC_DateStruct.Date);
    RTCDate_Numeric.X_Pos = 96;
    RTCDate_Numeric.Y_Pos = 4;
    RTCDate_Numeric.Width = 28;
    RTCDate_Numeric.Height = 20;
    RTCDate_Numeric.Value = RTC_DateStruct.Date;
    RTCDate_Numeric.Text = (const char *) String_Buffer_Tmp;
    RTCDate_Numeric.Widget_Type = Numeric_Widget_Type;
    RTCDate_Numeric.NextNumeric = &RTCHour_Numeric;
    RTCDate_Numeric.CallbackFunction = RTCYear_Numeric_Callback;
    GUI_InitNumeric(&RTCDate_Numeric);

    RTCTime_Lable.X_Pos = 2;
    RTCTime_Lable.Y_Pos = 30;
    RTCTime_Lable.Color = C_BLACK;
    RTCTime_Lable.Text = "Time";

    memset(String_Buffer_Tmp, 0x00, sizeof(String_Buffer_Tmp));
    sprintf(String_Buffer_Tmp, "%d", RTC_TimeStruct.Hours);
    RTCHour_Numeric.X_Pos = 26;
    RTCHour_Numeric.Y_Pos = 26;
    RTCHour_Numeric.Width = 30;
    RTCHour_Numeric.Height = 20;
    RTCHour_Numeric.Value = RTC_TimeStruct.Hours;
    RTCHour_Numeric.Text = (const char *) String_Buffer_Tmp;
    RTCHour_Numeric.Widget_Type = Numeric_Widget_Type;
    RTCHour_Numeric.NextNumeric = &RTCMinute_Numeric;
    RTCHour_Numeric.CallbackFunction = RTCYear_Numeric_Callback;
    GUI_InitNumeric(&RTCHour_Numeric);

    memset(String_Buffer_Tmp, 0x00, sizeof(String_Buffer_Tmp));
    sprintf(String_Buffer_Tmp, "%d", RTC_TimeStruct.Minutes);
    RTCMinute_Numeric.X_Pos = 60;
    RTCMinute_Numeric.Y_Pos = 26;
    RTCMinute_Numeric.Width = 30;
    RTCMinute_Numeric.Height = 20;
    RTCMinute_Numeric.Value = RTC_TimeStruct.Minutes;
    RTCMinute_Numeric.Text = (const char *) String_Buffer_Tmp;
    RTCMinute_Numeric.Widget_Type = Numeric_Widget_Type;
    RTCMinute_Numeric.NextNumeric = &RTCSecond_Numeric;
    RTCMinute_Numeric.CallbackFunction = RTCYear_Numeric_Callback;
    GUI_InitNumeric(&RTCMinute_Numeric);

    memset(String_Buffer_Tmp, 0x00, sizeof(String_Buffer_Tmp));
    sprintf(String_Buffer_Tmp, "%d", RTC_TimeStruct.Seconds);
    RTCSecond_Numeric.X_Pos = 94;
    RTCSecond_Numeric.Y_Pos = 26;
    RTCSecond_Numeric.Width = 30;
    RTCSecond_Numeric.Height = 20;
    RTCSecond_Numeric.Value = RTC_TimeStruct.Seconds;
    RTCSecond_Numeric.Text = (const char *) String_Buffer_Tmp;
    RTCSecond_Numeric.Widget_Type = Numeric_Widget_Type;
    RTCSecond_Numeric.NextNumeric = NULL;
    RTCSecond_Numeric.CallbackFunction = RTCYear_Numeric_Callback;
    GUI_InitNumeric(&RTCSecond_Numeric);

    GUI_InitLable(&RTCDate_Lable);
    GUI_InitLable(&RTCTime_Lable);
    GUI_InitButton(&TurnBack_Button2);
}