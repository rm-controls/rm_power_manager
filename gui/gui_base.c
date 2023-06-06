//
// Created by Lao·Zhu on 2023/5/17.
//

#include "gui_base.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "font.h"
#include "lcd.h"
#include "gpio.h"

__attribute__((section(".dma_ram")))volatile unsigned char lcd_frame_buffer[128 * 160 * 2] = {0};

void gui_config(void) {
    HAL_GPIO_WritePin(LCD_PWR_Port, LCD_PWR_Pin, GPIO_PIN_RESET);
    lcd_config();
    lcd_scan_mode(4);
}

unsigned short gui_trans_color(unsigned short color) {
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

void gui_putchar(unsigned char x,
                 unsigned char line,
                 unsigned char value,
                 unsigned int dcolor,
                 unsigned int bgcolor) {
    unsigned char i, j;
    unsigned char *temp = (unsigned char *) &Font_6_12[0];
    temp += (value - 32) * 12;
    for (j = 0; j < 12; j++) {
        for (i = 0; i < 6; i++) {
            if ((*temp & (1 << (7 - i))) != 0) {
                lcd_frame_buffer[((line + j) * 128 + x + i) * 2] = dcolor >> 8;
                lcd_frame_buffer[((line + j) * 128 + x + i) * 2 + 1] = dcolor & 0x00ff;
            } else {
                lcd_frame_buffer[((line + j) * 128 + x + i) * 2] = bgcolor >> 8;
                lcd_frame_buffer[((line + j) * 128 + x + i) * 2 + 1] = bgcolor & 0x00ff;
            }
        }
        temp++;
    }
}

void gui_printf(unsigned char row,
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
        gui_putchar(row, column, LCD_BUF[n], dcolor, bgcolor);
        row += 6;
        n++;
    }
}

void gui_clear_screen(unsigned short color) {
    for (unsigned short i = 0; i < 128 * 160; i++) {
        lcd_frame_buffer[i * 2] = color >> 8;
        lcd_frame_buffer[i * 2 + 1] = color & 0x00ff;
    }
}

void gui_draw_point(unsigned short x, unsigned short y, unsigned short color) {
    lcd_frame_buffer[(y * 128 + x) * 2] = color >> 8;
    lcd_frame_buffer[(y * 128 + x) * 2 + 1] = color & 0x00ff;
}

void gui_draw_hline(unsigned char x1, unsigned char y1, unsigned char width, unsigned short color) {
    for (unsigned char temp = 0; temp < width; temp++) {
        lcd_frame_buffer[(y1 * 128 + x1 + temp) * 2] = color >> 8;
        lcd_frame_buffer[(y1 * 128 + x1 + temp) * 2 + 1] = color & 0x00ff;
    }
}

void gui_draw_vline(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color) {
    for (unsigned char temp = 0; temp < height; temp++) {
        lcd_frame_buffer[((y1 + temp) * 128 + x1) * 2] = color >> 8;
        lcd_frame_buffer[((y1 + temp) * 128 + x1) * 2 + 1] = color & 0x00ff;
    }
}

void gui_draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned short color) {
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
        gui_draw_point(uRow, uCol, color);
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

void gui_draw_circle(unsigned char x0,
                     unsigned char y0,
                     unsigned char r,
                     unsigned short color,
                     Filled_Status_e filled) {
    if (filled == Filled) {
        gui_draw_circle_corner(x0, y0, r, GUI_DRAW_CIRCLE_TL, color, Filled);
        gui_draw_circle_corner(x0, y0, r, GUI_DRAW_CIRCLE_TR, color, Filled);
        gui_draw_circle_corner(x0, y0 - 1, r, GUI_DRAW_CIRCLE_BL, color, Filled);
        gui_draw_circle_corner(x0, y0 - 1, r, GUI_DRAW_CIRCLE_BR, color, Filled);
    } else {
        gui_draw_circle_corner(x0, y0, r, GUI_DRAW_CIRCLE_TL, color, UnFilled);
        gui_draw_circle_corner(x0 - 1, y0, r, GUI_DRAW_CIRCLE_TR, color, UnFilled);
        gui_draw_circle_corner(x0, y0 - 1, r, GUI_DRAW_CIRCLE_BL, color, UnFilled);
        gui_draw_circle_corner(x0 - 1, y0 - 1, r, GUI_DRAW_CIRCLE_BR, color, UnFilled);
    }
}

void gui_draw_rectangle(unsigned short sx,
                        unsigned short sy,
                        unsigned short width,
                        unsigned short height,
                        unsigned short color,
                        Filled_Status_e filled) {
    if (filled == Filled) {
        for (unsigned short temph = 0; temph < height; temph++) {
            for (unsigned short tempw = 0; tempw < width; tempw++) {
                lcd_frame_buffer[((sy + temph) * 128 + sx + tempw) * 2] = color >> 8;
                lcd_frame_buffer[((sy + temph) * 128 + sx + tempw) * 2 + 1] = color & 0x00ff;
            }
        }
    } else {
        gui_draw_hline(sx, sy, width, color);
        gui_draw_vline(sx, sy, height, color);
        gui_draw_hline(sx, sy + height - 1, width, color);
        gui_draw_vline(sx + width - 1, sy, height, color);
    }
}

void gui_draw_circle_corner(unsigned char x0,
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
                case GUI_DRAW_CIRCLE_TL:gui_draw_hline(x0 - x, y0 - y, x, color);
                    gui_draw_hline(x0 - y, y0 - x, y, color);
                    break;
                case GUI_DRAW_CIRCLE_TR:gui_draw_hline(x0, y0 - y, x, color);
                    gui_draw_hline(x0, y0 - x, y, color);
                    break;
                case GUI_DRAW_CIRCLE_BL: gui_draw_hline(x0 - y, y0 + x, y, color);
                    gui_draw_hline(x0 - x, y0 + y, x, color);
                    break;
                case GUI_DRAW_CIRCLE_BR:gui_draw_hline(x0, y0 + x, y, color);
                    gui_draw_hline(x0, y0 + y, x, color);
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
                case GUI_DRAW_CIRCLE_TL:gui_draw_point(x0 - y, y0 - x, color);
                    gui_draw_point(x0 - x, y0 - y, color);
                    break;
                case GUI_DRAW_CIRCLE_TR:gui_draw_point(x0 + x, y0 - y, color);
                    gui_draw_point(x0 + y, y0 - x, color);
                    break;
                case GUI_DRAW_CIRCLE_BL:gui_draw_point(x0 - x, y0 + y, color);
                    gui_draw_point(x0 - y, y0 + x, color);
                    break;
                case GUI_DRAW_CIRCLE_BR:gui_draw_point(x0 + x, y0 + y, color);
                    gui_draw_point(x0 + y, y0 + x, color);
                    break;
                default:break;
            }
        }
    }
}

void gui_draw_round_rectangle(unsigned char x,
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
                gui_draw_rectangle(x + r, y, width - 2 * r, height, color, Filled);
            if ((height - 2 * r) > 0) {
                gui_draw_rectangle(x, y + r, r, height - 2 * r, color, Filled);
                gui_draw_rectangle(x + width - r, y + r, r, height - 2 * r, color, Filled);
            }
            gui_draw_circle_corner(x + r, y + r, r, GUI_DRAW_CIRCLE_TL, color, Filled);
            gui_draw_circle_corner(x + width - r, y + r, r, GUI_DRAW_CIRCLE_TR, color, Filled);
            gui_draw_circle_corner(x + r, y + height - r - 1, r, GUI_DRAW_CIRCLE_BL, color, Filled);
            gui_draw_circle_corner(x + width - r, y + height - r - 1, r, GUI_DRAW_CIRCLE_BR, color, Filled);
        } else
            gui_draw_rectangle(x, y, width, height, color, Filled);
    } else {
        if (r >= (height / 2))
            r = height / 2 - 1;
        if (r >= (width / 2))
            r = width / 2 - 1;
        if (r) {
            gui_draw_hline(x + r, y, width - 2 * r, color);
            gui_draw_vline(x + width - 1, y + r, height - 2 * r, color);
            gui_draw_hline(x + r, y + height - 1, width - 2 * r, color);
            gui_draw_vline(x, y + r, height - 2 * r, color);
            gui_draw_circle_corner(x + r, y + r, r, GUI_DRAW_CIRCLE_TL, color, UnFilled);
            gui_draw_circle_corner(x + width - r - 1, y + r, r, GUI_DRAW_CIRCLE_TR, color, UnFilled);
            gui_draw_circle_corner(x + r, y + height - r - 1, r, GUI_DRAW_CIRCLE_BL, color, UnFilled);
            gui_draw_circle_corner(x + width - r - 1, y + height - r - 1, r, GUI_DRAW_CIRCLE_BR, color, UnFilled);
        } else
            gui_draw_rectangle(x, y, width, height, color, UnFilled);
    }
}

void gui_draw_triangle(unsigned char x1,
                       unsigned char y1,
                       unsigned char x2,
                       unsigned char y2,
                       unsigned char x3,
                       unsigned char y3,
                       unsigned short color,
                       Filled_Status_e filled) {
    if (filled == Filled) {
        short deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;
        deltax = (short) ((x2 - x1) < 0 ? -1 * (x2 - x1) : (x2 - x1));
        deltay = (short) ((y2 - y1) < 0 ? -1 * (y2 - y1) : (y2 - y1));
        x = x1;
        y = y1;
        if (x2 >= x1) {
            xinc1 = 1;
            xinc2 = 1;
        } else {
            xinc1 = -1;
            xinc2 = -1;
        }
        if (y2 >= y1) {
            yinc1 = 1;
            yinc2 = 1;
        } else {
            yinc1 = -1;
            yinc2 = -1;
        }
        if (deltax >= deltay) {
            xinc1 = 0;
            yinc2 = 0;
            den = deltax;
            num = (short) (deltax / 2);
            numadd = deltay;
            numpixels = deltax;
        } else {
            xinc2 = 0;
            yinc1 = 0;
            den = deltay;
            num = (short) (deltay / 2);
            numadd = deltax;
            numpixels = deltay;
        }
        for (curpixel = 0; curpixel <= numpixels; curpixel++) {
            gui_draw_line(x, y, x3, y3, color);
            num = (short) (num + numadd);
            if (num >= den) {
                num = (short) (num - den);
                x = (short) (x + xinc1);
                y = (short) (y + yinc1);
            }
            x = (short) (x + xinc2);
            y = (short) (y + yinc2);
        }
    } else {
        gui_draw_line(x1, y1, x2, y2, color);
        gui_draw_line(x1, y1, x3, y3, color);
        gui_draw_line(x2, y2, x3, y3, color);
    }
}
