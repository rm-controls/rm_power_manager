//
// Created by LaoZhu on 2023/6/6.
//

#include "main.h"

_Noreturn void digital_tube_task(void *parameters) {
    (void) parameters;
    static unsigned int running_timer_counter = 0;
    static unsigned char digital_tube_display_buffer[4] = {0};
    static mode_target_e last_mode = refresh_mode;
    while (1) {
        running_timer_counter++;
        mode_target_e current_mode = fsm_get_mode();
        if (last_mode != current_mode || running_timer_counter % 5 == 0) {
            digital_tube_display_buffer[0] = 0;
            digital_tube_display_buffer[1] = current_mode + (16 * (running_timer_counter % 10 == 0));
            tm1650_write_data(1, digital_tube_display_buffer);
        }
        HAL_IWDG_Refresh(&hiwdg1);
        delayms(100);
    }
}
