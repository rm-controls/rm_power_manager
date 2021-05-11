//
// Created by Lao·Zhu on 2021/5/12.
//

#include "main.h"

Button_Struct_t TurnBack_Button4;
Lable_Struct_t Referee_Online_Lable;

void TurnBack_Button4_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Log_Form_Index;
    LogForm_Init();
}

void SysInfoForm_Update(void) {
    if (referee_avaiflag == 1) {
        Referee_Online_Lable.Color = TransColor(C_DARK_GREEN);
        GUI_LableSetText(&Referee_Online_Lable, "Referee Online ");
    } else {
        Referee_Online_Lable.Color = TransColor(C_DARK_RED);
        GUI_LableSetText(&Referee_Online_Lable, "Referee Offline");
    }
}

void SysInfoForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button4;

    Referee_Online_Lable.X_Pos = 19;
    Referee_Online_Lable.Y_Pos = 2;
    Referee_Online_Lable.Color = C_DARK_RED;
    Referee_Online_Lable.Text = "Referee Offline";

    TurnBack_Button4.X_Pos = 24;
    TurnBack_Button4.Y_Pos = 136;
    TurnBack_Button4.Width = 80;
    TurnBack_Button4.Height = 20;
    TurnBack_Button4.Text = "TurnBack";
    TurnBack_Button4.NextButton = NULL;
    TurnBack_Button4.CallbackFunction = TurnBack_Button4_Callback;

    GUI_Clear(C_WHITE);
    GUI_InitButton(&TurnBack_Button4);
    GUI_InitLable(&Referee_Online_Lable);
}
