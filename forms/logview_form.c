//
// Created by Lao·Zhu on 2021/5/12.
//

#include "main.h"

static ListBox_Struct_t EventList_ListBox;
static Button_Struct_t TurnBack_Button5;

static void EventList_ListBox_Callback(void *object, unsigned char key) {}

static void TurnBack_Button5_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Log_Form_Index;
    LogForm_Init();
}

void LogViewForm_Init(unsigned char index) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &EventList_ListBox;

    EventList_ListBox.X_Pos = 4;
    EventList_ListBox.Y_Pos = 18;
    EventList_ListBox.Width = 120;
    EventList_ListBox.Height = 112;
    EventList_ListBox.NextListBox = &TurnBack_Button5;
    EventList_ListBox.Text = "FileList";
    EventList_ListBox.CallbackFunction = EventList_ListBox_Callback;

    TurnBack_Button5.X_Pos = 24;
    TurnBack_Button5.Y_Pos = 136;
    TurnBack_Button5.Width = 80;
    TurnBack_Button5.Height = 20;
    TurnBack_Button5.Text = "TurnBack";
    TurnBack_Button5.NextButton = NULL;
    TurnBack_Button5.CallbackFunction = TurnBack_Button5_Callback;

    gui_clear_screen(C_WHITE);
    GUI_InitListBox(&EventList_ListBox);
    GUI_InitButton(&TurnBack_Button5);
    GUI_ListBoxAddItem(&EventList_ListBox, "Index:%d", index);
}

void LogViewForm_Update(void) {
    LogForm_Update();
}
