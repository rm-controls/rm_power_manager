//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

static Button_Struct_t TurnBack_Button2, Normal_Button, Charge_Button, Boost_Button;

void TurnBack_Button2_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void Normal_Button_Callback(void *object, unsigned char key) {
    fsm_set_mode(normal_mode);
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void Charge_Button_Callback(void *object, unsigned char key) {
    fsm_set_mode(charge_mode);
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void Boost_Button_Callback(void *object, unsigned char key) {
    fsm_set_mode(boost_mode);
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void SettingsForm_Update(void) {}

void SettingsForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &Normal_Button;

    Normal_Button.X_Pos = 24;
    Normal_Button.Y_Pos = 10;
    Normal_Button.Width = 80;
    Normal_Button.Height = 20;
    Normal_Button.Text = "Normal";
    Normal_Button.NextButton = &Charge_Button;
    Normal_Button.CallbackFunction = Normal_Button_Callback;

    Charge_Button.X_Pos = 24;
    Charge_Button.Y_Pos = 40;
    Charge_Button.Width = 80;
    Charge_Button.Height = 20;
    Charge_Button.Text = "Charge";
    Charge_Button.NextButton = &Boost_Button;
    Charge_Button.CallbackFunction = Charge_Button_Callback;

    Boost_Button.X_Pos = 24;
    Boost_Button.Y_Pos = 70;
    Boost_Button.Width = 80;
    Boost_Button.Height = 20;
    Boost_Button.Text = "Boost";
    Boost_Button.NextButton = &TurnBack_Button2;
    Boost_Button.CallbackFunction = Boost_Button_Callback;

    TurnBack_Button2.X_Pos = 24;
    TurnBack_Button2.Y_Pos = 136;
    TurnBack_Button2.Width = 80;
    TurnBack_Button2.Height = 20;
    TurnBack_Button2.Text = "TurnBack";
    TurnBack_Button2.NextButton = NULL;
    TurnBack_Button2.CallbackFunction = TurnBack_Button2_Callback;

    gui_clear_screen(C_WHITE);
    GUI_InitButton(&Normal_Button);
    GUI_InitButton(&Charge_Button);
    GUI_InitButton(&Boost_Button);
    GUI_InitButton(&TurnBack_Button2);
}