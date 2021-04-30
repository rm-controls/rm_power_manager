#ifndef _GUI_H_
#define _GUI_H_

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

void GUI_Init(void);
unsigned short TransColor(unsigned short color);
void GUI_Clear(unsigned short color);
void GUI_Point(unsigned short x, unsigned short y, unsigned short color);
void GUI_DrawRectangle(unsigned short sx,
                       unsigned short sy,
                       unsigned short width,
                       unsigned short height,
                       unsigned short color,
                       Filled_Status_e filled);
void GUI_DrawCircleCorner(unsigned char x0,
                          unsigned char y0,
                          unsigned char r,
                          Circle_Corner_e c,
                          unsigned short color,
                          Filled_Status_e filled);
void GUI_DrawCircle(unsigned char x0,
                    unsigned char y0,
                    unsigned char r,
                    unsigned short color,
                    Filled_Status_e filled);
void GUI_DrawRoundRectangle(unsigned char x,
                            unsigned char y,
                            unsigned char width,
                            unsigned char height,
                            unsigned char r,
                            unsigned short color,
                            Filled_Status_e filled);
void GUI_DrawVLine(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color);
void GUI_DrawHLine(unsigned char x1, unsigned char y1, unsigned char height, unsigned short color);
void GUI_DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned short color);
void LCD_SendChar(unsigned char x,
                  unsigned char line,
                  unsigned char value,
                  unsigned int dcolor,
                  unsigned int bgcolor);
void GUI_Printf(unsigned char row,
                unsigned char column,
                unsigned int dcolor,
                unsigned int bgcolor,
                const char *fmt,
                ...);

#endif
