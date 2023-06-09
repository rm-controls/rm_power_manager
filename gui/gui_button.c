//
// Created by Lao·Zhu on 2021/4/29.
//

#include "gui_button.h"
#include "gui_base.h"
#include "string.h"
#include "form_logic.h"

void GUI_InitButton(Button_Struct_t *button) {
    unsigned char text_x = button->X_Pos + (button->Width - strlen(button->Text) * 6) / 2;
    unsigned char text_y = button->Y_Pos + (button->Height - 12) / 2;
    button->Status = Button_Normal;
    button->Widget_Type = Button_Widget_Type;
    gui_draw_rectangle(button->X_Pos,
                       button->Y_Pos,
                       button->Width,
                       button->Height,
                       BUTTON_COLOR_NORMAL,
                       Filled);
    gui_draw_rectangle(button->X_Pos,
                       button->Y_Pos,
                       button->Width,
                       button->Height,
                       BUTTON_COLOR_NORMAL_EDGE,
                       UnFilled);
    gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_NORMAL, "%s", button->Text);
}

void GUI_UpdateButton(Button_Struct_t *button, Button_State_e status) {
    unsigned char text_x = button->X_Pos + (button->Width - strlen(button->Text) * 6) / 2;
    unsigned char text_y = button->Y_Pos + (button->Height - 12) / 2;
    button->Status = status;
    switch (status) {
        case Button_Click:
            gui_draw_rectangle(button->X_Pos,
                               button->Y_Pos,
                               button->Width,
                               button->Height,
                               BUTTON_COLOR_CLICK,
                               Filled);
            gui_draw_rectangle(button->X_Pos,
                               button->Y_Pos,
                               button->Width,
                               button->Height,
                               BUTTON_COLOR_CLICK_EDGE,
                               UnFilled);
            gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_CLICK, "%s", button->Text);
            break;
        case Button_Normal:
            gui_draw_rectangle(button->X_Pos,
                               button->Y_Pos,
                               button->Width,
                               button->Height,
                               BUTTON_COLOR_NORMAL,
                               Filled);
            gui_draw_rectangle(button->X_Pos,
                               button->Y_Pos,
                               button->Width,
                               button->Height,
                               BUTTON_COLOR_NORMAL_EDGE,
                               UnFilled);
            gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_NORMAL, "%s", button->Text);
            break;
        case Button_Focus:
            gui_draw_rectangle(button->X_Pos,
                               button->Y_Pos,
                               button->Width,
                               button->Height,
                               BUTTON_COLOR_FOCUS,
                               Filled);
            gui_draw_rectangle(button->X_Pos,
                               button->Y_Pos,
                               button->Width,
                               button->Height,
                               BUTTON_COLOR_FOCUS_EDGE,
                               UnFilled);
            gui_printf(text_x, text_y, C_BLACK, BUTTON_COLOR_FOCUS, "%s", button->Text);
            break;
        default:break;
    }
}
