//
// Created by Lao·Zhu on 2021/4/29.
//

#include "gui_button.h"
#include "gui_base.h"
#include "string.h"

void GUI_Button(unsigned char x,
                unsigned char y,
                unsigned char width,
                unsigned char height,
                unsigned char *text,
                Button_State_e state) {
    unsigned char text_x = x + (width - strlen((const char *) text) * 8) / 2;
    unsigned char text_y = y + (height - 16) / 2;
    switch (state) {
        case Button_Click:GUI_DrawRectangle(x, y, width, height, BUTTON_COLOR_CLICK, Filled);
            GUI_DrawRectangle(x, y, width, height, BUTTON_COLOR_CLICK_EDGE, UnFilled);
            GUI_Printf(text_x, text_y, C_BLACK, BUTTON_COLOR_CLICK, "%s", text);
            break;
        case Button_Normal:GUI_DrawRectangle(x, y, width, height, BUTTON_COLOR_NORMAL, Filled);
            GUI_DrawRectangle(x, y, width, height, BUTTON_COLOR_NORMAL_EDGE, UnFilled);
            GUI_Printf(text_x, text_y, C_BLACK, BUTTON_COLOR_NORMAL, "%s", text);
            break;
        case Button_Focus:GUI_DrawRectangle(x, y, width, height, BUTTON_COLOR_FOCUS, Filled);
            GUI_DrawRectangle(x, y, width, height, BUTTON_COLOR_FOCUS_EDGE, UnFilled);
            GUI_Printf(text_x, text_y, C_BLACK, BUTTON_COLOR_FOCUS, "%s", text);
            break;
        default:break;
    }
}
