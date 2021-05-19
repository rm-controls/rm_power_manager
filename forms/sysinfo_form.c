//
// Created by Lao·Zhu on 2021/5/12.
//

#include "main.h"

Button_Struct_t TurnBack_Button4;
Lable_Struct_t Referee_Online_Lable, RwH_L_Lable;

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
    GUI_LableSetText(&RwH_L_Lable,
                     "Rl:%d.%dR Rh:%d.%dR",
                     (unsigned int) Capacitor_Calibratel.Rw,
                     (unsigned int) (10 * (Capacitor_Calibratel.Rw - (float) ((int) Capacitor_Calibratel.Rw))),
                     (unsigned int) Capacitor_Calibrateh.Rw,
                     (unsigned int) (10 * (Capacitor_Calibrateh.Rw - (float) ((int) Capacitor_Calibrateh.Rw))));
}

void SysInfoForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button4;

    Referee_Online_Lable.X_Pos = 19;
    Referee_Online_Lable.Y_Pos = 2;
    Referee_Online_Lable.Color = C_DARK_RED;
    Referee_Online_Lable.Text = "Referee Offline";

    RwH_L_Lable.X_Pos = 19;
    RwH_L_Lable.Y_Pos = 16;
    RwH_L_Lable.Color = C_BLACK;
    RwH_L_Lable.Text = "Rl:-.-R Rh:-.-R";

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
    GUI_InitLable(&RwH_L_Lable);
}
