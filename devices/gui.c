#include "lcd.h"
#include "gui.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "font.h"

void LCD_SendChar(unsigned char x,
                  unsigned char line,
                  unsigned char value,
                  unsigned int dcolor,
                  unsigned int bgcolor) {
    unsigned char i, j;
    dcolor = TransColor(dcolor);
    unsigned char *temp = (unsigned char *) &Font_8_16[0];
    temp += (value - 32) * 16;
    Address_set(x, line, x + 7, line + 15);
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 8; i++) {
            if ((*temp & (1 << (7 - i))) != 0)
                Lcd_Write_Data(dcolor);
            else
                Lcd_Write_Data(bgcolor);
        }
        temp++;
    }
}

void GUI_Printf(unsigned char row,
                unsigned char line,
                unsigned int dcolor,
                unsigned int bgcolor,
                const char *fmt,
                ...) {
    char LCD_BUF[32] = {0};
    unsigned char n = 0;
    va_list ap;
    memset(LCD_BUF, '\0', sizeof(LCD_BUF));
    va_start(ap, fmt);
    vsprintf(LCD_BUF, fmt, ap);
    va_end(ap);
    while (LCD_BUF[n] != '\0') {
        LCD_SendChar(row, line, LCD_BUF[n], dcolor, bgcolor);
        row += 8;
        n++;
    }
}

void GUI_Clear(unsigned short color) {
    unsigned char i;
    unsigned char j;
    Address_set(0, 0, 127, 159);
    color = TransColor(color);
    for (i = 0; i < 128; i++) {
        for (j = 0; j < 160; j++)
            Lcd_Write_Data(color);
    }
}

void GUI_Point(unsigned short x, unsigned short y, unsigned short color) {
    unsigned int Temp_Color = TransColor(color);
    Address_set(x, y, x, y);
    Lcd_Write_Data(Temp_Color);
}

unsigned short TransColor(unsigned short color) {
    unsigned int Result;
    unsigned int Temp_Red, Temp_Blue, Temp_Green;
    Temp_Red = color >> 11U;
    Temp_Blue = color & 0x001fU;
    Temp_Green = (color & 0x07ffU) >> 5U;
    Result = Temp_Red;
    Result |= Temp_Green << 5U;
    Result |= Temp_Blue << 11U;
    return Result;
}

void GUI_Rectangle(unsigned short sx,
                   unsigned short sy,
                   unsigned short ex,
                   unsigned short ey,
                   unsigned short color) {
    unsigned int temp, temp1, m, n;
    unsigned int Temp_Color = TransColor(color);
    Address_set(sx, sy, ex, ey);
    n = ex - sx + 1;
    m = ey - sy + 1;
    for (temp = 0; temp < n; temp++) {
        for (temp1 = 0; temp1 < m; temp1++) {
            Lcd_Write_Data(Temp_Color);
        }
    }
}
