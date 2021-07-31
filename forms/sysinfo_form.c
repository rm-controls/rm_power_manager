//
// Created by Lao·Zhu on 2021/5/12.
//

#include "main.h"

Button_Struct_t TurnBack_Button4;
Lable_Struct_t Referee_Online_Lable, X2_Coefficient_Lable, Setting_FSM_Lable;
Lable_Struct_t Referee_GameStatus_Lable, X_Coefficient_Lable;

unsigned char Setting_OptiSchemes = 0;

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
    GUI_LableSetText(&X2_Coefficient_Lable,
                     "%d.%d %d.%d",
                     (unsigned int) Capacitor_Calibrate.coefficient[0],
                     (unsigned int) (10000000
                         * (Capacitor_Calibrate.coefficient[0] - (float) ((int) Capacitor_Calibrate.coefficient[0]))),
                     (unsigned int) Capacitor_Calibrate.coefficient[1],
                     (unsigned int) (10000000
                         * (Capacitor_Calibrate.coefficient[1] - (float) ((int) Capacitor_Calibrate.coefficient[1]))));
    GUI_LableSetText(&X_Coefficient_Lable, "%d.%d", (unsigned int) Capacitor_Calibrate.coefficient[2],
                     (unsigned int) (10000000
                         * (Capacitor_Calibrate.coefficient[2] - (float) ((int) Capacitor_Calibrate.coefficient[2]))));

    switch (Setting_OptiSchemes) {
        case Normal_Optimized:GUI_LableSetText(&Setting_FSM_Lable, "  Optimize:  Normal  ");
            break;
        case ChargeFirst_Optimized:GUI_LableSetText(&Setting_FSM_Lable, "Optimize:Charge First");
            break;
        case UseFirst_Optimized:GUI_LableSetText(&Setting_FSM_Lable, " Optimize: Use First ");
            break;
        case SucapTest_Optimized:GUI_LableSetText(&Setting_FSM_Lable, " Optimize: SucapTest ");
            break;
        default:break;
    }

    switch (referee_data_.game_status_.game_progress) {
        case 0:GUI_LableSetText(&Referee_GameStatus_Lable, " GameStatus: NoStart ");
            break;
        case 1:GUI_LableSetText(&Referee_GameStatus_Lable, " GameStatus: Prepare ");
            break;
        case 2:GUI_LableSetText(&Referee_GameStatus_Lable, "GameStatus: SelfCheck");
            break;
        case 3:GUI_LableSetText(&Referee_GameStatus_Lable, "GameStatus: CountDown");
            break;
        case 4:GUI_LableSetText(&Referee_GameStatus_Lable, "GameStatus: Fighting ");
            break;
        case 5:GUI_LableSetText(&Referee_GameStatus_Lable, " GameStatus: Sumarry ");
            break;
        default:GUI_LableSetText(&Referee_GameStatus_Lable, "   GameStatus: NAN   ");
            break;
    }
}

void SysInfoForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button4;

    Referee_Online_Lable.X_Pos = 19;
    Referee_Online_Lable.Y_Pos = 2;
    Referee_Online_Lable.Color = C_DARK_RED;
    Referee_Online_Lable.Text = "Referee Offline";

    X2_Coefficient_Lable.X_Pos = 6;
    X2_Coefficient_Lable.Y_Pos = 16;
    X2_Coefficient_Lable.Color = C_BLACK;
    X2_Coefficient_Lable.Text = "-.- -.-";

    X_Coefficient_Lable.X_Pos = 33;
    X_Coefficient_Lable.Y_Pos = 30;
    X_Coefficient_Lable.Color = C_BLACK;
    X_Coefficient_Lable.Text = "-.-";

    Setting_FSM_Lable.X_Pos = 1;
    Setting_FSM_Lable.Y_Pos = 44;
    Setting_FSM_Lable.Color = C_BLACK;
    Setting_FSM_Lable.Text = "   Optimize: None    ";

    Referee_GameStatus_Lable.X_Pos = 1;
    Referee_GameStatus_Lable.Y_Pos = 58;
    Referee_GameStatus_Lable.Color = C_BLACK;
    Referee_GameStatus_Lable.Text = "   GameStatus: NAN   ";

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
    GUI_InitLable(&X2_Coefficient_Lable);
    GUI_InitLable(&Setting_FSM_Lable);
    GUI_InitLable(&Referee_GameStatus_Lable);
}
