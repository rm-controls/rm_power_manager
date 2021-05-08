//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

Button_Struct_t TurnBack_Button1, Format_Button;
Lable_Struct_t DateTime_Lable;
ListBox_Struct_t FileList_ListBox;

void TurnBack_Button1_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void Format_Button_Callback(void *object, unsigned char key) {
    FileSystem_FormatFlash();
    FileSystem_FindRemainSpace();
    FileSystem_Structure->FirstFileAddr = CurrentFile_Address;
    FileSystem_Structure->LastFileAddr = CurrentFile_Address;
}

void FileList_ListBox_Callback(void *object, unsigned char key) {

}

void LogForm_Update(void) {
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BIN);
    GUI_LableSetText(&DateTime_Lable,
                     "20%02d-%02d-%02d %02d:%02d:%02d",
                     RTC_DateStruct.Year,
                     RTC_DateStruct.Month,
                     RTC_DateStruct.Date,
                     RTC_TimeStruct.Hours,
                     RTC_TimeStruct.Minutes,
                     RTC_TimeStruct.Seconds);
}

void GUI_ListBox_ScanFile() {
    if (FileSystem_Structure->FileNum != 1) {
        FileHead_Struct_t CurrentFile_Tmp;
        unsigned int NextHeadAddr = FileSystem_Structure->FirstFileAddr;
        for (unsigned char counter = 0; counter < FileSystem_Structure->FileNum - 1; counter++) {
            W25QXX_Read((unsigned char *) &CurrentFile_Tmp, NextHeadAddr, sizeof(FileHead_Struct_t));
            GUI_ListBoxAddItem(&FileList_ListBox,
                               "20%d/%d/%d-%02d:%02d:%02d",
                               CurrentFile_Tmp.Year,
                               CurrentFile_Tmp.Month,
                               CurrentFile_Tmp.Day,
                               CurrentFile_Tmp.Hour,
                               CurrentFile_Tmp.Minute,
                               CurrentFile_Tmp.Second);
            NextHeadAddr = CurrentFile_Tmp.NextFileAddr;
        }
    } else
        GUI_ListBoxAddItem(&FileList_ListBox, "No Log Item");
}

void LogForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button1;

    DateTime_Lable.Color = C_BLACK;
    DateTime_Lable.X_Pos = 4;
    DateTime_Lable.Y_Pos = 3;
    DateTime_Lable.Text = "Date Time Now";

    FileList_ListBox.X_Pos = 4;
    FileList_ListBox.Y_Pos = 18;
    FileList_ListBox.Width = 120;
    FileList_ListBox.Height = 76;
    FileList_ListBox.NextListBox = NULL;
    FileList_ListBox.Text = "FileList";
    FileList_ListBox.CallbackFunction = FileList_ListBox_Callback;

    TurnBack_Button1.X_Pos = 2;
    TurnBack_Button1.Y_Pos = 135;
    TurnBack_Button1.Width = 60;
    TurnBack_Button1.Height = 20;
    TurnBack_Button1.Text = "TurnBack";
    TurnBack_Button1.NextButton = &Format_Button;
    TurnBack_Button1.CallbackFunction = TurnBack_Button1_Callback;

    Format_Button.X_Pos = 64;
    Format_Button.Y_Pos = 135;
    Format_Button.Width = 60;
    Format_Button.Height = 20;
    Format_Button.Text = "Format";
    Format_Button.NextButton = &FileList_ListBox;
    Format_Button.CallbackFunction = Format_Button_Callback;

    GUI_Clear(C_WHITE);
    GUI_InitListBox(&FileList_ListBox);
    GUI_InitLable(&DateTime_Lable);
    GUI_InitButton(&TurnBack_Button1);
    GUI_InitButton(&Format_Button);
    GUI_ListBox_ScanFile();

    GUI_Printf(4, 96, C_BLACK, C_WHITE, "OS:%2d.%1dKB", xPortGetFreeHeapSize() / 1024,
               (xPortGetFreeHeapSize() % 1024) / 100);
    GUI_Printf(64, 96, C_BLACK, C_WHITE, "FS:%03d/256", FileSystem_Structure->FileNum);
}
