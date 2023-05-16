//
// Created by LaoZhu on 2023/5/12.
//

#include "main.h"
#include "protect.h"

extern volatile unsigned char uart2_receive_buffer1[UART_DMA_BUFFER_SIZE];

void protect_task(void *parameters) {
    (void) parameters;
    static unsigned int cycle_counter = 0, overpower_flag = 0;
    while (1) {
        cycle_counter = cycle_counter + 1;

        /* RTC time calibrate. */
        if ((cycle_counter % ((RTC_TIME_CALIBRATE_PERIOD * 1000) / PROTECT_TASK_PERIOD)) == 0) {
            static datetime_t current_datetime = {0};
            utc_second_to_date_time(referee_info.timestamp, &current_datetime);
            rtc_set_date(current_datetime.year - 2000, current_datetime.month, current_datetime.day, 1);
            rtc_set_time(current_datetime.hour, current_datetime.minute, current_datetime.second);
        }

        /* Over power detect and protect. */
        if (power_info.chassis_power > 220.0f && overpower_flag == 0)
            overpower_flag = 1;
        else if (power_info.chassis_power > 220.0f && overpower_flag == 1) {
            fsm_set_mode(all_off_mode);
        }

        if (referee_available() == 0)
            HAL_UART_Receive_DMA(&huart2, (unsigned char *) uart2_receive_buffer1, UART_DMA_BUFFER_SIZE);

        delayms(PROTECT_TASK_PERIOD);
    }
}
