//
// Created by Lao·Zhu on 2021/5/1.
//

#include "form_logic.h"
#include "main_form.h"
#include "gui_button.h"
#include "key.h"
#include "stddef.h"
#include "log_form.h"
#include "settings_form.h"

Form_Info_t Form_Info_Structure;
GUI_Object_Struct_t *FirstWidget;

void Form_UpdateEvent(void) {
    Key_Press_t key_num = ReadKey();
    GUI_Object_Struct_t *Current_Widget = FirstWidget, *Tmp_Widget = NULL;
    for (unsigned char counter = 0; counter < Form_Info_Structure.Widget_Index; counter++)
        Current_Widget = Current_Widget->NextObject;
    if (key_num.Last_Num == Up_Key && key_num.Num == 0)
        if (Form_Info_Structure.Widget_Index != 0)
            Form_Info_Structure.Widget_Index--;
    if (key_num.Last_Num == Down_Key && key_num.Num == 0)
        if (Current_Widget->NextObject != NULL)
            Form_Info_Structure.Widget_Index++;
    if (key_num.Last_Num == 0 && key_num.Num == Center_Key && Current_Widget->Widget_Type == Button_Widget_Type)
        GUI_UpdateButton((Button_Struct_t *) Current_Widget, Button_Click);
    else if (key_num.Last_Num == Center_Key && key_num.Num == 0 && Current_Widget->Widget_Type == Button_Widget_Type) {
        GUI_UpdateButton((Button_Struct_t *) Current_Widget, Button_Focus);
        Current_Widget->CallbackFunction(Current_Widget, key_num.Num);
    }
    Current_Widget = FirstWidget;
    Tmp_Widget = FirstWidget;
    for (unsigned char counter = 0; counter < Form_Info_Structure.Widget_Index; counter++)
        Current_Widget = Current_Widget->NextObject;
    while (1) {
        if (((Button_Struct_t *) Tmp_Widget)->Status != Button_Normal && Tmp_Widget != Current_Widget
            && Current_Widget->Widget_Type == Button_Widget_Type)
            GUI_UpdateButton((Button_Struct_t *) Tmp_Widget, Button_Normal);
        if (Tmp_Widget->NextObject == NULL)
            break;
        else
            Tmp_Widget = Tmp_Widget->NextObject;
    }
    if (((Button_Struct_t *) Current_Widget)->Status == Button_Normal
        && Current_Widget->Widget_Type == Button_Widget_Type)
        GUI_UpdateButton((Button_Struct_t *) Current_Widget, Button_Focus);

    switch (Form_Info_Structure.Form_Index) {
        case Main_Form_Index:MainForm_Update();
            break;
        case Log_Form_Index:LogForm_Update();
            break;
        case Settings_Form_Index:SettingsForm_Update();
            break;
    }
}
