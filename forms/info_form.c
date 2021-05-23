//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

Button_Struct_t TurnBack_Button1, Format_Button, OSInfo_Button, SysInfo_Button;
Lable_Struct_t DateTime_Lable;
ListBox_Struct_t FileList_ListBox;

void SysInfo_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = SysInfo_Form_Index;
    SysInfoForm_Init();
}

void OSInfo_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = OSInfo_Form_Index;
    OSInfoForm_Init();
}

void TurnBack_Button1_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void Format_Button_Callback(void *object, unsigned char key) {
    if (FileSystem_Available_Flag != 0) {
        FileSystem_FormatFlash();
        if (CurrentFile_Structure != NULL)
            vPortFree(CurrentFile_Structure);
        FileSystem_CreateFiles();
    }
}

void FileList_ListBox_Callback(void *object, unsigned char key) {
    if (key == Center_Key) {
        Form_Info_Structure.Form_Index = LogView_Form_Index;
        LogViewForm_Init(((ListBox_Struct_t *) object)->ItemIndex);
    }
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

void GFileList_ListBox_ScanFile() {
    if (FileSystem_Structure->FileNum != 1 && FileSystem_Available_Flag != 0) {
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
    } else if (FileSystem_Available_Flag != 0)
        GUI_ListBoxAddItem(&FileList_ListBox, "No Log Item");
    else
        GUI_ListBoxAddItem(&FileList_ListBox, "Flash Broken");
}

void LogForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &FileList_ListBox;

    DateTime_Lable.Color = C_BLACK;
    DateTime_Lable.X_Pos = 4;
    DateTime_Lable.Y_Pos = 3;
    DateTime_Lable.Text = "Date Time Now";

    memset(&FileList_ListBox, 0x00, sizeof(ListBox_Struct_t));
    FileList_ListBox.X_Pos = 4;
    FileList_ListBox.Y_Pos = 18;
    FileList_ListBox.Width = 120;
    FileList_ListBox.Height = 76;
    FileList_ListBox.NextListBox = &OSInfo_Button;
    FileList_ListBox.Text = "FileList";
    FileList_ListBox.CallbackFunction = FileList_ListBox_Callback;

    OSInfo_Button.X_Pos = 2;
    OSInfo_Button.Y_Pos = 113;
    OSInfo_Button.Width = 60;
    OSInfo_Button.Height = 20;
    OSInfo_Button.Text = "OS Info";
    OSInfo_Button.NextButton = &Format_Button;
    OSInfo_Button.CallbackFunction = OSInfo_Button_Callback;

    Format_Button.X_Pos = 64;
    Format_Button.Y_Pos = 113;
    Format_Button.Width = 60;
    Format_Button.Height = 20;
    Format_Button.Text = "Format";
    Format_Button.NextButton = &SysInfo_Button;
    Format_Button.CallbackFunction = Format_Button_Callback;

    SysInfo_Button.X_Pos = 2;
    SysInfo_Button.Y_Pos = 135;
    SysInfo_Button.Width = 60;
    SysInfo_Button.Height = 20;
    SysInfo_Button.Text = "SysInfo";
    SysInfo_Button.NextButton = &TurnBack_Button1;
    SysInfo_Button.CallbackFunction = SysInfo_Button_Callback;

    TurnBack_Button1.X_Pos = 64;
    TurnBack_Button1.Y_Pos = 135;
    TurnBack_Button1.Width = 60;
    TurnBack_Button1.Height = 20;
    TurnBack_Button1.Text = "TurnBack";
    TurnBack_Button1.NextButton = NULL;
    TurnBack_Button1.CallbackFunction = TurnBack_Button1_Callback;

    GUI_Clear(C_WHITE);
    GUI_InitListBox(&FileList_ListBox);
    GUI_InitLable(&DateTime_Lable);
    GUI_InitButton(&OSInfo_Button);
    GUI_InitButton(&Format_Button);
    GUI_InitButton(&SysInfo_Button);
    GUI_InitButton(&TurnBack_Button1);
    GFileList_ListBox_ScanFile();

    GUI_Printf(4, 96, C_BLACK, C_WHITE, "OS:%2d.%1dKB", xPortGetFreeHeapSize() / 1024,
               (xPortGetFreeHeapSize() % 1024) / 100);
    GUI_Printf(64, 96, C_BLACK, C_WHITE, "FS:%03d/256", FileSystem_Structure->FileNum);
}
