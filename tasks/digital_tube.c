//
// Created by LaoZhu on 2023/6/6.
//

#include "main.h"

void digital_tube_task(void *parameters) {
    (void) parameters;
    static unsigned int running_timer_counter = 0;
    static unsigned char digital_tube_display_buffer[4] = {0};
    while (1) {
        running_timer_counter++;
        if (running_timer_counter % 10 == 0) {
            digital_tube_display_buffer[0] = (running_timer_counter / 100) % 32;
            digital_tube_display_buffer[1] = (running_timer_counter / 10) % 32;
            tm1650_write_data(1, digital_tube_display_buffer);
        }
        HAL_IWDG_Refresh(&hiwdg1);
        delayms(100);
    }
}
