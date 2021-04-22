#ifndef _GUI_H_
#define _GUI_H_

#include "color.h"

unsigned short TransColor(unsigned short color);
void GUI_Clear(unsigned short color);
void GUI_Point(unsigned short x, unsigned short y, unsigned short color);
void GUI_Rectangle(unsigned short sx,
                   unsigned short sy,
                   unsigned short ex,
                   unsigned short ey,
                   unsigned short color);
void GUI_Printf(unsigned char row,
                unsigned char line,
                unsigned int dcolor,
                unsigned int bgcolor,
                const char *fmt,
                ...);

#endif
