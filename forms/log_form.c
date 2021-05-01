//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

Button_Struct_t TurnBack_Button1;

void TurnBack_Button1_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void LogForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button1;

    TurnBack_Button1.X_Pos = 24;
    TurnBack_Button1.Y_Pos = 135;
    TurnBack_Button1.Width = 80;
    TurnBack_Button1.Height = 20;
    TurnBack_Button1.Text = "Turn Back";
    TurnBack_Button1.NextButton = NULL;
    TurnBack_Button1.CallbackFunction = TurnBack_Button1_Callback;

    GUI_Clear(C_WHITE);
    GUI_InitButton(&TurnBack_Button1);
}

void LogForm_Update(void) {

}
