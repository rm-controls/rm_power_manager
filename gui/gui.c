#include "lcd.h"
#include "gui.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "font.h"

void GUI_Init(void) {
    LCD_Config();
    LCD_Scan(4);
    GUI_Clear(C_WHITE);
}

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
                unsigned char column,
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
        LCD_SendChar(row, column, LCD_BUF[n], dcolor, bgcolor);
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

void GUI_DrawHLine(unsigned char x1, unsigned char y1, unsigned char width, unsigned short color) {
    unsigned char temp;
    color = TransColor(color);
    Address_set(x1, y1, x1 + width - 1, y1);
    for (temp = 0; temp < width; temp++)
        Lcd_Write_Data(color);
}

void GUI_DrawVLine(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color) {
    unsigned char temp;
    color = TransColor(color);
    Address_set(x1, y1, x1, y1 + height - 1);
    for (temp = 0; temp < height; temp++)
        Lcd_Write_Data(color);
}

void GUI_DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned short color) {
    unsigned short t;
    short xerr = 0, yerr = 0, delta_x, delta_y, distance;
    short incx, incy, uRow, uCol;
    delta_x = (short) (x2 - x1);
    delta_y = (short) (y2 - y1);
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)incx = 1;
    else if (delta_x == 0)incx = 0;
    else {
        incx = -1;
        delta_x = (short) -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0;
    else {
        incy = -1;
        delta_y = (short) -delta_y;
    }
    if (delta_x > delta_y)distance = delta_x;
    else distance = delta_y;
    for (t = 0; t <= distance + 1; t++) {
        GUI_Point(uRow, uCol, color);
        xerr = (short) (xerr + delta_x);
        yerr = (short) (yerr + delta_y);
        if (xerr > distance) {
            xerr = (short) (xerr - distance);
            uRow = (short) (uRow + incx);
        }
        if (yerr > distance) {
            yerr = (short) (yerr - distance);
            uCol = (short) (uCol + incy);
        }
    }
}

void GUI_DrawCircle(unsigned char x0,
                    unsigned char y0,
                    unsigned char r,
                    unsigned short color,
                    Filled_Status_e filled) {
    if (filled == Filled) {
        GUI_DrawCircleCorner(x0, y0, r, GUI_DRAW_CIRCLE_TL, color, Filled);
        GUI_DrawCircleCorner(x0, y0, r, GUI_DRAW_CIRCLE_TR, color, Filled);
        GUI_DrawCircleCorner(x0, y0 - 1, r, GUI_DRAW_CIRCLE_BL, color, Filled);
        GUI_DrawCircleCorner(x0, y0 - 1, r, GUI_DRAW_CIRCLE_BR, color, Filled);
    } else {
        GUI_DrawCircleCorner(x0, y0, r, GUI_DRAW_CIRCLE_TL, color, UnFilled);
        GUI_DrawCircleCorner(x0 - 1, y0, r, GUI_DRAW_CIRCLE_TR, color, UnFilled);
        GUI_DrawCircleCorner(x0, y0 - 1, r, GUI_DRAW_CIRCLE_BL, color, UnFilled);
        GUI_DrawCircleCorner(x0 - 1, y0 - 1, r, GUI_DRAW_CIRCLE_BR, color, UnFilled);
    }
}

void GUI_DrawRectangle(unsigned short sx,
                       unsigned short sy,
                       unsigned short width,
                       unsigned short height,
                       unsigned short color,
                       Filled_Status_e filled) {
    if (filled == Filled) {
        unsigned short temp, temp1;
        color = TransColor(color);
        Address_set(sx, sy, sx + width - 1, sy + height - 1);
        for (temp = 0; temp < width; temp++)
            for (temp1 = 0; temp1 < height; temp1++)
                Lcd_Write_Data(color);
    } else {
        GUI_DrawHLine(sx, sy, width, color);
        GUI_DrawVLine(sx, sy, height, color);
        GUI_DrawHLine(sx, sy + height - 1, width, color);
        GUI_DrawVLine(sx + width - 1, sy, height, color);
    }
}

void GUI_DrawCircleCorner(unsigned char x0,
                          unsigned char y0,
                          unsigned char r,
                          Circle_Corner_e c,
                          unsigned short color,
                          Filled_Status_e filled) {
    if (filled == Filled) {
        short f = (short) (1 - r);
        short ddF_x = 1;
        short ddF_y = (short) (-2 * r);
        short x = 0;
        short y = r;
        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f = (short) (f + ddF_y);
            }
            x++;
            ddF_x += 2;
            f = (short) (f + ddF_x);
            switch (c) {
                case GUI_DRAW_CIRCLE_TL:GUI_DrawHLine(x0 - x, y0 - y, x, color);
                    GUI_DrawHLine(x0 - y, y0 - x, y, color);
                    break;
                case GUI_DRAW_CIRCLE_TR:GUI_DrawHLine(x0, y0 - y, x, color);
                    GUI_DrawHLine(x0, y0 - x, y, color);
                    break;
                case GUI_DRAW_CIRCLE_BL: GUI_DrawHLine(x0 - y, y0 + x, y, color);
                    GUI_DrawHLine(x0 - x, y0 + y, x, color);
                    break;
                case GUI_DRAW_CIRCLE_BR:GUI_DrawHLine(x0, y0 + x, y, color);
                    GUI_DrawHLine(x0, y0 + y, x, color);
                    break;
                default:break;
            }
        }
    } else {
        short f = (short) (1 - r);
        short ddF_x = 1;
        short ddF_y = (short) (-2 * r);
        short x = 0;
        short y = r;
        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f = (short) (f + ddF_y);
            }
            x++;
            ddF_x += 2;
            f = (short) (f + ddF_x);
            switch (c) {
                case GUI_DRAW_CIRCLE_TL:GUI_Point(x0 - y, y0 - x, color);
                    GUI_Point(x0 - x, y0 - y, color);
                    break;
                case GUI_DRAW_CIRCLE_TR:GUI_Point(x0 + x, y0 - y, color);
                    GUI_Point(x0 + y, y0 - x, color);
                    break;
                case GUI_DRAW_CIRCLE_BL:GUI_Point(x0 - x, y0 + y, color);
                    GUI_Point(x0 - y, y0 + x, color);
                    break;
                case GUI_DRAW_CIRCLE_BR:GUI_Point(x0 + x, y0 + y, color);
                    GUI_Point(x0 + y, y0 + x, color);
                    break;
                default:break;
            }
        }
    }
}

void GUI_DrawRoundRectangle(unsigned char x,
                            unsigned char y,
                            unsigned char width,
                            unsigned char height,
                            unsigned char r,
                            unsigned short color,
                            Filled_Status_e filled) {
    if (filled == Filled) {
        if (r >= (height / 2))
            r = height / 2 - 1;
        if (r >= (width / 2))
            r = width / 2 - 1;
        if (r) {
            if ((width - 2 * r) > 0)
                GUI_DrawRectangle(x + r, y, width - 2 * r, height, color, Filled);
            if ((height - 2 * r) > 0) {
                GUI_DrawRectangle(x, y + r, r, height - 2 * r, color, Filled);
                GUI_DrawRectangle(x + width - r, y + r, r, height - 2 * r, color, Filled);
            }
            GUI_DrawCircleCorner(x + r, y + r, r, GUI_DRAW_CIRCLE_TL, color, Filled);
            GUI_DrawCircleCorner(x + width - r, y + r, r, GUI_DRAW_CIRCLE_TR, color, Filled);
            GUI_DrawCircleCorner(x + r, y + height - r - 1, r, GUI_DRAW_CIRCLE_BL, color, Filled);
            GUI_DrawCircleCorner(x + width - r, y + height - r - 1, r, GUI_DRAW_CIRCLE_BR, color, Filled);
        } else
            GUI_DrawRectangle(x, y, width, height, color, Filled);
    } else {
        if (r >= (height / 2))
            r = height / 2 - 1;
        if (r >= (width / 2))
            r = width / 2 - 1;
        if (r) {
            GUI_DrawHLine(x + r, y, width - 2 * r, color);
            GUI_DrawVLine(x + width - 1, y + r, height - 2 * r, color);
            GUI_DrawHLine(x + r, y + height - 1, width - 2 * r, color);
            GUI_DrawVLine(x, y + r, height - 2 * r, color);
            GUI_DrawCircleCorner(x + r, y + r, r, GUI_DRAW_CIRCLE_TL, color, UnFilled);
            GUI_DrawCircleCorner(x + width - r - 1, y + r, r, GUI_DRAW_CIRCLE_TR, color, UnFilled);
            GUI_DrawCircleCorner(x + r, y + height - r - 1, r, GUI_DRAW_CIRCLE_BL, color, UnFilled);
            GUI_DrawCircleCorner(x + width - r - 1, y + height - r - 1, r, GUI_DRAW_CIRCLE_BR, color, UnFilled);
        } else
            GUI_DrawRectangle(x, y, width, height, color, UnFilled);
    }
}
