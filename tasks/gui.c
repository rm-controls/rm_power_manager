//
// Created by LaoZhu on 2023/5/17.
//

#include "main.h"
#include "gui.h"

extern volatile unsigned char lcd_frame_buffer[128 * 160 * 2];

void gui_task(void *parameters) {
    (void) parameters;
    static unsigned int running_timer_counter = 0;
    gui_clear_screen(C_WHITE);
    gui_printf(22, 74, C_DARK_GREEN, C_WHITE, "Calibrating...");
    lcd_refresh_once(lcd_frame_buffer);
    delayms(1000);
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
    while (1) {
        Form_UpdateEvent();
        running_timer_counter++;
        if (running_timer_counter == 10)
            gui_draw_circle(123, 5, 4, C_WHITE, Filled);
        else if (running_timer_counter == 20) {
            running_timer_counter = 0;
            if (referee_available())
                gui_draw_circle(123, 5, 4, C_DARK_GREEN, Filled);
            else
                gui_draw_circle(123, 5, 4, C_DARK_RED, Filled);
        }
        lcd_refresh_once(lcd_frame_buffer);
        delayms(100);
    }
}
