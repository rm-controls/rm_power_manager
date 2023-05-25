//
// Created by LaoZhu on 2023/5/26.
//

#include "main.h"
#include "selfcheck.h"

#define SELFCHECK_CURRENT_MINIMUM   (SELF_CHECK_CURRENT_MIDDLE_VOLTAGE - SELF_CHECK_CURRENT_TOLERANCE)
#define SELFCHECK_CURRENT_MAXIMUM   (SELF_CHECK_CURRENT_MIDDLE_VOLTAGE + SELF_CHECK_CURRENT_TOLERANCE)

static char textbox_line_buffer[32] = {0};

void slefcheck_current_sensor(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char current_sensor_check_error_flag = 0;
    switch (step) {
        case 1:fsm_set_mode(selftest_all_off_mode);
            break;
        case 5:
            if (calibrate_params.chassis_current_offset<SELFCHECK_CURRENT_MINIMUM ||
                calibrate_params.chassis_current_offset>SELFCHECK_CURRENT_MAXIMUM) {
                GUI_TextBoxAppend(textbox, C_DARK_RED, "Chassis_cur mid err");
                current_sensor_check_error_flag++;
            }
            if (calibrate_params.charge_current_offset<SELFCHECK_CURRENT_MINIMUM ||
                calibrate_params.charge_current_offset>SELFCHECK_CURRENT_MAXIMUM) {
                GUI_TextBoxAppend(textbox, C_DARK_RED, "Charge_cur mid err");
                current_sensor_check_error_flag++;
            }
            break;
        case 10:
            if (current_sensor_check_error_flag == 0)
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Current sensor pass");
            break;
        default:break;
    }
}

void slefcheck_voltage_sensor(TextBox_Struct_t *textbox, unsigned char step) {

}

void slefcheck_passthrough_components(TextBox_Struct_t *textbox, unsigned char step) {

}

void slefcheck_charge_components(TextBox_Struct_t *textbox, unsigned char step) {

}

void slefcheck_boost_components(TextBox_Struct_t *textbox, unsigned char step) {

}

void slefcheck_referee_status(TextBox_Struct_t *textbox, unsigned char step) {

}

void slefcheck_nuc_status(TextBox_Struct_t *textbox, unsigned char step) {

}
