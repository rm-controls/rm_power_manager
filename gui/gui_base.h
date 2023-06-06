//
// Created by Lao·Zhu on 2021/2/26.
//

#ifndef POWER_MANAGER_GUI_GUI_BASE_H_
#define POWER_MANAGER_GUI_GUI_BASE_H_

#include "color.h"

typedef enum {
  Filled = 0,
  UnFilled = 1,
} Filled_Status_e;

typedef enum {
  GUI_DRAW_CIRCLE_TL = 0,
  GUI_DRAW_CIRCLE_TR,
  GUI_DRAW_CIRCLE_BL,
  GUI_DRAW_CIRCLE_BR,
} Circle_Corner_e;

void gui_config(void);
unsigned short gui_trans_color(unsigned short color);
void gui_draw_point(unsigned short x, unsigned short y, unsigned short color);
void gui_clear_screen(unsigned short color);
void gui_draw_rectangle(unsigned short sx,
                        unsigned short sy,
                        unsigned short width,
                        unsigned short height,
                        unsigned short color,
                        Filled_Status_e filled);
void gui_draw_circle_corner(unsigned char x0,
                            unsigned char y0,
                            unsigned char r,
                            Circle_Corner_e c,
                            unsigned short color,
                            Filled_Status_e filled);
void gui_draw_circle(unsigned char x0,
                     unsigned char y0,
                     unsigned char r,
                     unsigned short color,
                     Filled_Status_e filled);
void gui_draw_round_rectangle(unsigned char x,
                              unsigned char y,
                              unsigned char width,
                              unsigned char height,
                              unsigned char r,
                              unsigned short color,
                              Filled_Status_e filled);
void gui_draw_vline(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color);
void gui_draw_hline(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color);
void gui_draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned short color);
void gui_putchar(unsigned char x,
                 unsigned char line,
                 unsigned char value,
                 unsigned int dcolor,
                 unsigned int bgcolor);
void gui_printf(unsigned char row,
                unsigned char column,
                unsigned int dcolor,
                unsigned int bgcolor,
                const char *fmt,
                ...);
void gui_draw_triangle(unsigned char x1,
                       unsigned char y1,
                       unsigned char x2,
                       unsigned char y2,
                       unsigned char x3,
                       unsigned char y3,
                       unsigned short color,
                       Filled_Status_e filled);

#endif  // POWER_MANAGER_GUI_GUI_BASE_H_
