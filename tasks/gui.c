//
// Created by LaoZhu on 2023/5/17.
//

#include "main.h"
#include "gui.h"

extern volatile unsigned char lcd_frame_buffer[128 * 160 * 2];

void gui_task(void *parameters) {
    (void) parameters;
    while (1) {
        lcd_refresh_once(lcd_frame_buffer);
        delayms(100);
    }
}
