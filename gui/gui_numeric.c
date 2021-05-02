//
// Created by Lao·Zhu on 2021/5/1.
//

#include "gui_numeric.h"
#include "gui_base.h"
#include "gui_button.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

void GUI_NumericSetText(Numeric_Struct_t *Numeric, const char *fmt, ...) {
    char LCD_BUF[64], n = 0, row = Numeric->X_Pos + 3;
    va_list ap;
    memset(LCD_BUF, '\0', sizeof(LCD_BUF));
    va_start(ap, fmt);
    vsprintf(LCD_BUF, fmt, ap);
    va_end(ap);
    GUI_DrawRectangle(Numeric->X_Pos + 3,
                      Numeric->Y_Pos + (Numeric->Height - 12) / 2,
                      Numeric->Width - 16,
                      12,
                      C_WHITE,
                      Filled);
    while (LCD_BUF[n] != '\0') {
        LCD_SendChar(row, Numeric->Y_Pos + (Numeric->Height - 12) / 2, LCD_BUF[n], C_BLACK, C_WHITE);
        row += 6;
        n++;
    }
}

void GUI_InitNumeric(Numeric_Struct_t *Numeric) {
    unsigned char btn_x_pos = Numeric->X_Pos + Numeric->Width - 13;
    unsigned char btn_up_y_pos = Numeric->Y_Pos + 2;
    unsigned char btn_down_pos = Numeric->Y_Pos + Numeric->Height - 2 - (Numeric->Height - 5) / 2;
    unsigned char btn_height = (Numeric->Height - 5) / 2;
    Numeric->Status = Numeric_Button_Normal;
    GUI_DrawRectangle(Numeric->X_Pos, Numeric->Y_Pos, Numeric->Width, Numeric->Height, C_WHITE, Filled);
    GUI_DrawRectangle(Numeric->X_Pos, Numeric->Y_Pos, Numeric->Width, Numeric->Height, NUMERIC_NORMAL_EDGE, UnFilled);
    GUI_Printf(Numeric->X_Pos + 3, Numeric->Y_Pos + (Numeric->Height - 12) / 2, C_BLACK, C_WHITE, "%s", Numeric->Text);
    GUI_DrawRectangle(btn_x_pos, btn_up_y_pos, 11, btn_height, BUTTON_COLOR_NORMAL, Filled);
    GUI_DrawRectangle(btn_x_pos, btn_up_y_pos, 11, btn_height, BUTTON_COLOR_NORMAL_EDGE, UnFilled);
    GUI_DrawTriangle(btn_x_pos + 5,
                     btn_up_y_pos + 1,
                     btn_x_pos + 2,
                     btn_up_y_pos + btn_height - 3,
                     btn_x_pos + 8,
                     btn_up_y_pos + btn_height - 3,
                     C_BLACK,
                     Filled);
    GUI_DrawRectangle(btn_x_pos, btn_down_pos, 11, btn_height, BUTTON_COLOR_NORMAL, Filled);
    GUI_DrawRectangle(btn_x_pos, btn_down_pos, 11, btn_height, BUTTON_COLOR_NORMAL_EDGE, UnFilled);
    GUI_DrawTriangle(btn_x_pos + 5,
                     btn_down_pos + btn_height - 2,
                     btn_x_pos + 2,
                     btn_down_pos + 2,
                     btn_x_pos + 8,
                     btn_down_pos + 2,
                     C_BLACK,
                     Filled);
}

void GUI_UpdateNumeric(Numeric_Struct_t *Numeric, Numeric_Button_State_e btn_state) {
    unsigned char btn_x_pos = Numeric->X_Pos + Numeric->Width - 13;
    unsigned char btn_up_y_pos = Numeric->Y_Pos + 2;
    unsigned char btn_down_pos = Numeric->Y_Pos + Numeric->Height - 2 - (Numeric->Height - 5) / 2;
    unsigned char btn_height = (Numeric->Height - 5) / 2;
    Numeric->Status = btn_state;
    switch (btn_state) {
        case Numeric_Button_Normal:
            GUI_DrawRectangle(btn_x_pos,
                              btn_up_y_pos,
                              11,
                              btn_height,
                              BUTTON_COLOR_NORMAL,
                              Filled);
            GUI_DrawRectangle(btn_x_pos, btn_up_y_pos, 11, btn_height, BUTTON_COLOR_NORMAL_EDGE, UnFilled);
            GUI_DrawTriangle(btn_x_pos + 5,
                             btn_up_y_pos + 1,
                             btn_x_pos + 2,
                             btn_up_y_pos + btn_height - 3,
                             btn_x_pos + 8,
                             btn_up_y_pos + btn_height - 3,
                             C_BLACK,
                             Filled);
            GUI_DrawRectangle(btn_x_pos,
                              btn_down_pos,
                              11,
                              btn_height,
                              BUTTON_COLOR_NORMAL,
                              Filled);
            GUI_DrawRectangle(btn_x_pos, btn_down_pos, 11, btn_height, BUTTON_COLOR_NORMAL_EDGE, UnFilled);
            GUI_DrawTriangle(btn_x_pos + 5,
                             btn_down_pos + btn_height - 2,
                             btn_x_pos + 2,
                             btn_down_pos + 2,
                             btn_x_pos + 8,
                             btn_down_pos + 2,
                             C_BLACK,
                             Filled);
            break;
        case Numeric_Up_Button_Focus:
            GUI_DrawRectangle(btn_x_pos,
                              btn_up_y_pos,
                              11,
                              btn_height,
                              BUTTON_COLOR_FOCUS,
                              Filled);
            GUI_DrawRectangle(btn_x_pos, btn_up_y_pos, 11, btn_height, BUTTON_COLOR_FOCUS_EDGE, UnFilled);
            GUI_DrawTriangle(btn_x_pos + 5,
                             btn_up_y_pos + 1,
                             btn_x_pos + 2,
                             btn_up_y_pos + btn_height - 3,
                             btn_x_pos + 8,
                             btn_up_y_pos + btn_height - 3,
                             C_BLACK,
                             Filled);
            break;
        case Numeric_Up_Button_Click:
            GUI_DrawRectangle(btn_x_pos,
                              btn_down_pos,
                              11,
                              btn_height,
                              BUTTON_COLOR_NORMAL,
                              Filled);
            GUI_DrawRectangle(btn_x_pos, btn_down_pos, 11, btn_height, BUTTON_COLOR_NORMAL_EDGE, UnFilled);
            GUI_DrawTriangle(btn_x_pos + 5,
                             btn_down_pos + btn_height - 2,
                             btn_x_pos + 2,
                             btn_down_pos + 2,
                             btn_x_pos + 8,
                             btn_down_pos + 2,
                             C_BLACK,
                             Filled);
            GUI_DrawRectangle(btn_x_pos,
                              btn_up_y_pos,
                              11,
                              btn_height,
                              BUTTON_COLOR_CLICK,
                              Filled);
            GUI_DrawRectangle(btn_x_pos, btn_up_y_pos, 11, btn_height, BUTTON_COLOR_CLICK_EDGE, UnFilled);
            GUI_DrawTriangle(btn_x_pos + 5,
                             btn_up_y_pos + 1,
                             btn_x_pos + 2,
                             btn_up_y_pos + btn_height - 3,
                             btn_x_pos + 8,
                             btn_up_y_pos + btn_height - 3,
                             C_BLACK,
                             Filled);
            break;
        case Numeric_Down_Button_Focus:
            GUI_DrawRectangle(btn_x_pos,
                              btn_down_pos,
                              11,
                              btn_height,
                              BUTTON_COLOR_FOCUS,
                              Filled);
            GUI_DrawRectangle(btn_x_pos, btn_down_pos, 11, btn_height, BUTTON_COLOR_FOCUS_EDGE, UnFilled);
            GUI_DrawTriangle(btn_x_pos + 5,
                             btn_down_pos + btn_height - 2,
                             btn_x_pos + 2,
                             btn_down_pos + 2,
                             btn_x_pos + 8,
                             btn_down_pos + 2,
                             C_BLACK,
                             Filled);
            break;
        case Numeric_Down_Button_Click:
            GUI_DrawRectangle(btn_x_pos,
                              btn_up_y_pos,
                              11,
                              btn_height,
                              BUTTON_COLOR_NORMAL,
                              Filled);
            GUI_DrawRectangle(btn_x_pos, btn_up_y_pos, 11, btn_height, BUTTON_COLOR_NORMAL_EDGE, UnFilled);
            GUI_DrawTriangle(btn_x_pos + 5,
                             btn_up_y_pos + 1,
                             btn_x_pos + 2,
                             btn_up_y_pos + btn_height - 3,
                             btn_x_pos + 8,
                             btn_up_y_pos + btn_height - 3,
                             C_BLACK,
                             Filled);
            GUI_DrawRectangle(btn_x_pos,
                              btn_down_pos,
                              11,
                              btn_height,
                              BUTTON_COLOR_CLICK,
                              Filled);
            GUI_DrawRectangle(btn_x_pos, btn_down_pos, 11, btn_height, BUTTON_COLOR_CLICK_EDGE, UnFilled);
            GUI_DrawTriangle(btn_x_pos + 5,
                             btn_down_pos + btn_height - 2,
                             btn_x_pos + 2,
                             btn_down_pos + 2,
                             btn_x_pos + 8,
                             btn_down_pos + 2,
                             C_BLACK,
                             Filled);
            break;
        default:break;
    }
}
