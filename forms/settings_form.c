//
// Created by Lao·Zhu on 2021/5/1.
//

#include "main.h"

Button_Struct_t TurnBack_Button2;
Numeric_Struct_t Numeric_Test;

void TurnBack_Button2_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

void Numeric_Test_Callback(void *object, unsigned char key) {
    if (key == Right_Key)
        ((Numeric_Struct_t *) object)->Value++;
    else if (((Numeric_Struct_t *) object)->Value != 0)
        ((Numeric_Struct_t *) object)->Value--;
    GUI_NumericSetText((Numeric_Struct_t *) object, "%d", ((Numeric_Struct_t *) object)->Value);
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
    TurnBack_Button2.NextButton = &Numeric_Test;
    TurnBack_Button2.CallbackFunction = TurnBack_Button2_Callback;

    Numeric_Test.X_Pos = 10;
    Numeric_Test.Y_Pos = 10;
    Numeric_Test.Width = 40;
    Numeric_Test.Height = 20;
    Numeric_Test.Value = 0;
    Numeric_Test.Text = "0";
    Numeric_Test.Widget_Type = Numeric_Widget_Type;
    Numeric_Test.NextNumeric = NULL;
    Numeric_Test.CallbackFunction = Numeric_Test_Callback;

    GUI_Clear(C_WHITE);
    GUI_InitButton(&TurnBack_Button2);
    GUI_InitNumeric(&Numeric_Test);
}