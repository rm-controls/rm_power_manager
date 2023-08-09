//
// Created by LaoZhu on 2023/6/6.
//

#include "main.h"

extern unsigned char lcd_digital_tube_check(unsigned int delay_xms, unsigned char gui_task_call);

_Noreturn void digital_tube_task(void *parameters) {
    (void) parameters;
    static unsigned int running_timer_counter = 0, last_push_time = 0;
    static unsigned char digital_tube_display_buffer[4] = {0};
    static mode_target_e last_mode = refresh_mode;
    static typology_e last_typo = refresh_typology;
    iic_config();
    digital_tube_key_config();
    tm1650_config();
    tm1650_write_data(1, digital_tube_display_buffer);
    while (1) {
        running_timer_counter++;
        mode_target_e current_mode = fsm_get_mode();
        typology_e current_typo = fsm_get_typology();
        if (last_mode != current_mode || last_typo != current_typo || running_timer_counter % 5 == 0) {
            digital_tube_display_buffer[0] = current_typo + (16 * (running_timer_counter % 10 == 0));
            digital_tube_display_buffer[1] = current_mode + (16 * (running_timer_counter % 10 == 0));
            tm1650_write_data(1, digital_tube_display_buffer);
        }
        last_typo = current_typo;
        last_mode = current_mode;

        Key_Press_t key_press = ReadKey_DigitalTube();
        if (key_press.Num == User_Key && key_press.Last_Num == No_Key)
            last_push_time = HAL_GetTick();
        if (key_press.Num == No_Key && key_press.Last_Num == User_Key
            && ((HAL_GetTick() - last_push_time) > 3000)) {
            digital_tube_display_buffer[0] = 0x0F;
            digital_tube_display_buffer[1] = 0x0F;
            tm1650_write_data(1, digital_tube_display_buffer);
            fsm_set_mode(selftest_mode);
            vTaskSuspend(fsm_task_handler);
            for (;;) {
                HAL_IWDG_Refresh(&hiwdg1);
                unsigned short check_message = selfcheck_digital_tube();
                if (check_message == 0x0000)
                    break;
                digital_tube_display_buffer[0] = check_message >> 8;
                digital_tube_display_buffer[1] = check_message & 0x00FF;
                tm1650_write_data(1, digital_tube_display_buffer);
                delayms(100);
            }
            last_push_time = HAL_GetTick();
        }

        if (lcd_digital_tube_check(100, 0) == 1)
            vTaskDelete(NULL);
    }
}
