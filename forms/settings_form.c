//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

static Button_Struct_t TurnBack_Button2;

void TurnBack_Button2_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void SettingsForm_Update(void) {}

void SettingsForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button2;

    TurnBack_Button2.X_Pos = 2;
    TurnBack_Button2.Y_Pos = 134;
    TurnBack_Button2.Width = 80;
    TurnBack_Button2.Height = 22;
    TurnBack_Button2.Text = "Exit";
    TurnBack_Button2.NextButton = NULL;
    TurnBack_Button2.CallbackFunction = TurnBack_Button2_Callback;

    gui_clear_screen(C_WHITE);
    GUI_InitButton(&TurnBack_Button2);
}