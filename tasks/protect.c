//
// Created by LaoZhu on 2023/5/12.
//

#include "main.h"
#include "protect.h"

extern volatile unsigned char uart2_receive_buffer1[REFEREE_DMA_BUFFER_SIZE];
extern volatile unsigned char uart1_receive_buffer1[NUC_DMA_BUFFER_SIZE];
static unsigned int usart2_receive_timeout_counter = 0;

static struct protect_item_t {
  unsigned char over_current_flag;
  unsigned char under_voltage_flag;
  unsigned char usart1_error_flag;
  unsigned char usart2_error_flag;
} protect_info = {0};

unsigned char protect_get_info(void) {
    if (protect_info.over_current_flag != 0)
        return 1;
    if (protect_info.under_voltage_flag != 0)
        return 2;
    if (protect_info.usart2_error_flag != 0)
        return 3;
    return 0;
}

_Noreturn void protect_task(void *parameters) {
    (void) parameters;
    static unsigned int cycle_counter = 0;
    while (1) {
        cycle_counter = cycle_counter + 1;

        /* RTC time calibrate. */
        if ((cycle_counter % ((RTC_TIME_CALIBRATE_PERIOD * 1000) / PROTECT_TASK_PERIOD)) == 0) {
            static datetime_t current_datetime = {0};
            if (referee_info.timestamp >= CURRENT_TIME_UTC) {
                utc_second_to_date_time(referee_info.timestamp, &current_datetime);
                rtc_set_date(current_datetime.year - 2000, current_datetime.month, current_datetime.day, 1);
                rtc_set_time(current_datetime.hour, current_datetime.minute, current_datetime.second);
            }
        }

        /* Over power detect and protect. */
        if (power_info.chassis_power > 240.0f && protect_info.over_current_flag == 0) {
            delayms(100);
            if (power_info.chassis_power > 240.0f) {
                protect_info.over_current_flag = 1;
                fsm_set_mode(all_off_mode);
            }
        }

        /* Under voltage detect and restart. */
        if (power_info.battery_voltage < 18.0f && protect_info.under_voltage_flag == 0) {
            delayms(100);
            if (power_info.battery_voltage < 18.0f) {
                fsm_set_mode(all_off_mode);
                protect_info.under_voltage_flag = 1;
            }
        } else if (power_info.battery_voltage >= 20.0f && protect_info.under_voltage_flag == 1) {
            HAL_PWR_EnableBkUpAccess();
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0X83838383);
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, *((unsigned int *) &calibrate_params.current_k));
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, *((unsigned int *) &calibrate_params.current_b));
            soft_reset();
        }

        if (referee_available() == 0) {
            HAL_UART_Receive_DMA(&huart2, (unsigned char *) uart2_receive_buffer1, REFEREE_DMA_BUFFER_SIZE);
            usart2_receive_timeout_counter++;
            referee_info.chassis_power_buffer = 5;
            if (usart2_receive_timeout_counter >= 40)
                protect_info.usart2_error_flag = 1;
        } else {
            usart2_receive_timeout_counter = 0;
            protect_info.usart2_error_flag = 0;
        }

        if (nuc_available() == 0)
            HAL_UART_Receive_DMA(&huart1, (unsigned char *) uart1_receive_buffer1, NUC_DMA_BUFFER_SIZE);

        delayms(PROTECT_TASK_PERIOD);
    }
}
