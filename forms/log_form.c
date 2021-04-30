//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

Button_Struct_t TurnBack_Button;
extern Button_Struct_t *FirstButton;

void TurnBack_Button_Callback(Button_Struct_t *button) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void LogForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstButton = &TurnBack_Button;

    TurnBack_Button.X_Pos = 24;
    TurnBack_Button.Y_Pos = 135;
    TurnBack_Button.Width = 80;
    TurnBack_Button.Height = 20;
    TurnBack_Button.Text = "Turn Back";
    TurnBack_Button.NextButton = NULL;
    TurnBack_Button.CallbackFunction = TurnBack_Button_Callback;

    GUI_Clear(C_WHITE);
    GUI_InitButton(&TurnBack_Button);
}

void LogForm_Update(void) {

}
