//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

static Button_Struct_t TurnBack_Button1, Format_Button, OSInfo_Button, SysInfo_Button;
static Lable_Struct_t DateTime_Lable;
static ListBox_Struct_t FileList_ListBox;

static void SysInfo_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = SysInfo_Form_Index;
    SysInfoForm_Init();
}

static void OSInfo_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = OSInfo_Form_Index;
    OSInfoForm_Init();
}

static void TurnBack_Button1_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

static void Format_Button_Callback(void *object, unsigned char key) {
    make_rmfs_table();
}

static void FileList_ListBox_Callback(void *object, unsigned char key) {
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
    unsigned char fs_status = get_rmfs_status();
    switch (fs_status) {
        case 0:GUI_ListBoxAddItem(&FileList_ListBox, "First");
            break;
        case 1:GUI_ListBoxAddItem(&FileList_ListBox, "No Flash Found");
            break;
        case 2:GUI_ListBoxAddItem(&FileList_ListBox, "FS Table Error");
            break;
        default:GUI_ListBoxAddItem(&FileList_ListBox, "Unknown Error");
            break;
    }
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

    gui_clear_screen(C_WHITE);
    GUI_InitListBox(&FileList_ListBox);
    GUI_InitLable(&DateTime_Lable);
    GUI_InitButton(&OSInfo_Button);
    GUI_InitButton(&Format_Button);
    GUI_InitButton(&SysInfo_Button);
    GUI_InitButton(&TurnBack_Button1);
    GFileList_ListBox_ScanFile();

    gui_printf(4, 96, C_BLACK, C_WHITE, "OS:%2d.%1dKB", xPortGetFreeHeapSize() / 1024,
               (xPortGetFreeHeapSize() % 1024) / 100);
    gui_printf(80, 96, C_BLACK, C_WHITE, "FS:%04d", get_rmfs_remain());
}
