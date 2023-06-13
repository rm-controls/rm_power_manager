//
// Created by LaoZhu on 2023/6/6.
//

#include "tm1650.h"
#include "iic.h"
#include "gpio.h"

static const unsigned char digital_tube_segment_cmd[4] = {0x68, 0x6A, 0x6C, 0x6E};
static const unsigned char digital_tube_number_data[32] =
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
     0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
     0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87,
     0xFF, 0xDF, 0xF7, 0xFC, 0xB9, 0xDE, 0xF9, 0xF1};

void tm1650_config(void) {
    static unsigned char clear_tube_number[4] = {0};
    tm1650_write_data(0, clear_tube_number);
}

static unsigned char last_display_status = 0x00;
void tm1650_write_data(unsigned char disp_on, const unsigned char *data) {
    if ((disp_on & 0x01) == 0x00)
        HAL_GPIO_WritePin(LCD_PWR_Port, LCD_PWR_Pin, GPIO_PIN_SET);
    else {
        if (last_display_status == 0x00) {
            HAL_GPIO_WritePin(LCD_PWR_Port, LCD_PWR_Pin, GPIO_PIN_RESET);
            delayms(100);
        }
        iic_start();
        iic_send_byte(0x48);
        if (iic_wait_ack())
            return;
        iic_send_byte((disp_on & 0x01));
        if (iic_wait_ack())
            return;
        iic_stop();

        for (unsigned char counter = 0; counter < 4; ++counter) {
            iic_start();
            iic_send_byte(digital_tube_segment_cmd[counter]);
            if (iic_wait_ack())
                return;
            iic_send_byte(digital_tube_number_data[data[counter]]);
            if (iic_wait_ack())
                return;
            iic_stop();
        }
    }
    last_display_status = (disp_on & 0x01);
}
