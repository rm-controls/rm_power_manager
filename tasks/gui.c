//
// Created by LaoZhu on 2023/5/17.
//

#include "main.h"
#include "gui.h"

extern volatile unsigned char lcd_frame_buffer[128 * 160 * 2];
extern unsigned char lcd_digital_tube_check(unsigned int delay_xms, unsigned char gui_task_call);

static void form_config(void) {
    lcd_key_config();
    gui_clear_screen(C_WHITE);
    gui_printf(22, 74, C_DARK_GREEN, C_WHITE, "Calibrating...");
    HAL_IWDG_Refresh(&hiwdg1);
    gui_config();
    spi_setspeed(&hspi3, SPI_BAUDRATEPRESCALER_4);
    lcd_refresh_once(lcd_frame_buffer);
    for (int counter = 0; counter < 20; ++counter) {
        HAL_IWDG_Refresh(&hiwdg1);
        delayms(50);
    }
    Form_Info_Structure.Form_Index = Main_Form_Index;
    MainForm_Init();
}

_Noreturn void gui_task(void *parameters) {
    (void) parameters;
    static unsigned int running_timer_counter = 0;
    form_config();
    while (1) {
        HAL_IWDG_Refresh(&hiwdg1);
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
        Form_UpdateEvent();
        if (lcd_off_flag != 1)
            lcd_refresh_once(lcd_frame_buffer);
        if (lcd_digital_tube_check(100, 1) == 0)
            vTaskDelete(NULL);
    }
}
