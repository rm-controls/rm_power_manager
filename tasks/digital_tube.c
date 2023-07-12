//
// Created by LaoZhu on 2023/6/6.
//

#include "main.h"

extern unsigned char lcd_digital_tube_check(unsigned int delay_xms);

_Noreturn void digital_tube_task(void *parameters) {
    (void) parameters;
    static unsigned int running_timer_counter = 0;
    static unsigned char digital_tube_display_buffer[4] = {0};
    static mode_target_e last_mode = refresh_mode;
    static typology_e last_typo = refresh_typology;
    iic_config();
    digital_tube_key_config();
    tm1650_config();
    while (1) {
        running_timer_counter++;
        mode_target_e current_mode = fsm_get_mode();
        typology_e current_typo = fsm_get_typology();
        if (last_mode != current_mode || last_typo != current_typo || running_timer_counter % 5 == 0) {
            unsigned char current_status = (current_mode & 0x03) | ((current_typo & 0x03) << 2);
            digital_tube_display_buffer[0] = 0;
            digital_tube_display_buffer[1] = current_status + (16 * (running_timer_counter % 10 == 0));
            tm1650_write_data(1, digital_tube_display_buffer);
        }
        last_typo = current_typo;
        last_mode = current_mode;
        if (lcd_digital_tube_check(100) == 1)
            vTaskDelete(NULL);
    }
}
