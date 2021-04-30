//
// Created by Lao·Zhu on 2021/5/1.
//

#include "form_logic.h"
#include "main_form.h"
#include "gui_button.h"
#include "key.h"
#include "stddef.h"
#include "log_form.h"

Form_Info_t Form_Info_Structure;
Button_Struct_t *FirstButton;

void Form_UpdateEvent(void) {
    Key_Press_t key_num = ReadKey();
    Button_Struct_t *Current_Button = FirstButton, *Tmp_Button = NULL;
    for (unsigned char counter = 0; counter < Form_Info_Structure.Widget_Index; counter++)
        Current_Button = Current_Button->NextButton;
    if (key_num.Last_Num == Up_Key && key_num.Num == 0)
        if (Form_Info_Structure.Widget_Index != 0)
            Form_Info_Structure.Widget_Index--;
    if (key_num.Last_Num == Down_Key && key_num.Num == 0)
        if (Current_Button->NextButton != NULL)
            Form_Info_Structure.Widget_Index++;
    if (key_num.Last_Num == 0 && key_num.Num == Center_Key)
        GUI_UpdateButton(Current_Button, Button_Click);
    else if (key_num.Last_Num == Center_Key && key_num.Num == 0) {
        GUI_UpdateButton(Current_Button, Button_Focus);
        Current_Button->CallbackFunction(Current_Button);
    }
    Current_Button = FirstButton;
    Tmp_Button = FirstButton;
    for (unsigned char counter = 0; counter < Form_Info_Structure.Widget_Index; counter++)
        Current_Button = Current_Button->NextButton;
    while (1) {
        if (Tmp_Button->Status != Button_Normal && Tmp_Button != Current_Button)
            GUI_UpdateButton(Tmp_Button, Button_Normal);
        if (Tmp_Button->NextButton == NULL)
            break;
        else
            Tmp_Button = Tmp_Button->NextButton;
    }
    if (Current_Button->Status == Button_Normal)
        GUI_UpdateButton(Current_Button, Button_Focus);

    switch (Form_Info_Structure.Form_Index) {
        case Main_Form_Index:MainForm_Update();
            break;
        case Log_Form_Index:LogForm_Update();
            break;
    }
}
