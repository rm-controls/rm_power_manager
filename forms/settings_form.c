//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

Button_Struct_t TurnBack_Button2;

void TurnBack_Button2_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void SettingsForm_Update(void) {

}

void SettingsForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button2;
    TurnBack_Button2.X_Pos = 24;
    TurnBack_Button2.Y_Pos = 135;
    TurnBack_Button2.Width = 80;
    TurnBack_Button2.Height = 20;
    TurnBack_Button2.Text = "Turn Back";
    TurnBack_Button2.NextButton = NULL;
    TurnBack_Button2.CallbackFunction = TurnBack_Button2_Callback;

    GUI_Clear(C_WHITE);
    GUI_InitButton(&TurnBack_Button2);
}