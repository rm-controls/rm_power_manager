//
// Created by Lao·Zhu on 2021/5/12.
//

#include "main.h"

Button_Struct_t TurnBack_Button4;
Lable_Struct_t Referee_Online_Lable, RwH_L_Lable, Setting_FSM_Lable;
unsigned char Setting_FSM_Mode = 0;

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

    switch (Setting_FSM_Mode) {
        case 0:GUI_LableSetText(&Setting_FSM_Lable, " FSM Status: 0  ");
            break;
        case Normal_Mode:GUI_LableSetText(&Setting_FSM_Lable, " FSM Status: Normal  ");
            break;
        case OverPower_Mode:GUI_LableSetText(&Setting_FSM_Lable, "FSM Status: OverPower");
            break;
        case Halt_Mode:GUI_LableSetText(&Setting_FSM_Lable, "  FSM Status: Halt   ");
            break;
        case Transition_Mode:GUI_LableSetText(&Setting_FSM_Lable, " FSM Status: Transit ");
            break;
        default:break;
    }
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

    Setting_FSM_Lable.X_Pos = 1;
    Setting_FSM_Lable.Y_Pos = 30;
    Setting_FSM_Lable.Color = C_BLACK;
    Setting_FSM_Lable.Text = "  FSM Status: Halt   ";

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
    GUI_InitLable(&Setting_FSM_Lable);
}
