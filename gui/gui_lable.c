//
// Created by Lao·Zhu on 2021/5/1.
//

#include "gui_lable.h"
#include "gui_base.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

void GUI_InitLable(Lable_Struct_t *lable) {
    lable->Color = TransColor(lable->Color);
    char n = 0, row = lable->X_Pos;
    while (lable->Text[n] != '\0') {
        LCD_SendChar(row, lable->Y_Pos, lable->Text[n], lable->Color, C_WHITE);
        row += 6;
        n++;
    }
}

void GUI_LableSetText(Lable_Struct_t *lable, const char *fmt, ...) {
    char LCD_BUF[24], n = 0, row = lable->X_Pos;
    va_list ap;
    memset(LCD_BUF, '\0', sizeof(LCD_BUF));
    va_start(ap, fmt);
    vsprintf(LCD_BUF, fmt, ap);
    va_end(ap);
    while (LCD_BUF[n] != '\0') {
        LCD_SendChar(row, lable->Y_Pos, LCD_BUF[n], lable->Color, C_WHITE);
        row += 6;
        n++;
    }
}
