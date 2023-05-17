//
// Created by Lao·Zhu on 2021/5/12.
//

#include "main.h"

Button_Struct_t TurnBack_Button4;
Lable_Struct_t Referee_Online_Lable, Coefficient_Lable;
Lable_Struct_t Referee_GameStatus_Lable;

void TurnBack_Button4_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Log_Form_Index;
    LogForm_Init();
}

void SysInfoForm_Update(void) {
    if (referee_available() == 1) {
        Referee_Online_Lable.Color = C_DARK_GREEN;
        GUI_LableSetText(&Referee_Online_Lable, "Referee Online ");
    } else {
        Referee_Online_Lable.Color = C_DARK_RED;
        GUI_LableSetText(&Referee_Online_Lable, "Referee Offline");
    }

    GUI_LableSetText(&Coefficient_Lable,
                     "%d.%07d %d.%07d",
                     (unsigned int) calibrate_params.current_k,
                     (unsigned int) (10000000
                         * (calibrate_params.current_k - (float) ((int) calibrate_params.current_k))),
                     (unsigned int) calibrate_params.current_b,
                     (unsigned int) (10000000
                         * (calibrate_params.current_b - (float) ((int) calibrate_params.current_b))));

    switch (referee_info.game_progress) {
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

    Coefficient_Lable.X_Pos = 6;
    Coefficient_Lable.Y_Pos = 16;
    Coefficient_Lable.Color = C_BLACK;
    Coefficient_Lable.Text = "-.------- -.-------";

    Referee_GameStatus_Lable.X_Pos = 1;
    Referee_GameStatus_Lable.Y_Pos = 30;
    Referee_GameStatus_Lable.Color = C_BLACK;
    Referee_GameStatus_Lable.Text = "   GameStatus: NAN   ";

    TurnBack_Button4.X_Pos = 24;
    TurnBack_Button4.Y_Pos = 136;
    TurnBack_Button4.Width = 80;
    TurnBack_Button4.Height = 20;
    TurnBack_Button4.Text = "TurnBack";
    TurnBack_Button4.NextButton = NULL;
    TurnBack_Button4.CallbackFunction = TurnBack_Button4_Callback;

    gui_clear_screen(C_WHITE);
    GUI_InitButton(&TurnBack_Button4);
    GUI_InitLable(&Referee_Online_Lable);
    GUI_InitLable(&Referee_GameStatus_Lable);
}
