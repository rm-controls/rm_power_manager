//
// Created by LaoZhu on 2023/5/26.
//

#include "main.h"
#include "selfcheck.h"

#define SELFCHECK_CURRENT_MINIMUM   (SELF_CHECK_CURRENT_MIDDLE_VOLTAGE - SELF_CHECK_CURRENT_TOLERANCE)
#define SELFCHECK_CURRENT_MAXIMUM   (SELF_CHECK_CURRENT_MIDDLE_VOLTAGE + SELF_CHECK_CURRENT_TOLERANCE)

static char textbox_line_buffer[32] = {0};

extern unsigned short adc_result[6];

unsigned char slefcheck_current_sensor(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char current_sensor_check_error_flag = 0;
    switch (step) {
        case 1:pid_set_expect(0);
            close_all_switches();
            break;
        case 3:
            if (adc_result[1] < SELFCHECK_CURRENT_MINIMUM ||
                adc_result[1] > SELFCHECK_CURRENT_MAXIMUM) {
                sprintf(textbox_line_buffer, "cha_cur mid %04d", adc_result[1]);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
                current_sensor_check_error_flag++;
            }
            if (adc_result[0] < SELFCHECK_CURRENT_MINIMUM ||
                adc_result[0] > SELFCHECK_CURRENT_MAXIMUM) {
                sprintf(textbox_line_buffer, "cap_cur mid %04d", adc_result[0]);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
                current_sensor_check_error_flag++;
            }
            break;
            /* TODO: 此处可以多加一个步骤，即与裁判系统返回的电流进行比较，进一步验证电流传感器是否正确 */
        case 10:
            if (current_sensor_check_error_flag == 0)
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Current Sensor Pass");
            close_all_switches();
            break;
        default:break;
    }
    return current_sensor_check_error_flag;
}

unsigned char slefcheck_voltage_sensor(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char voltage_sensor_check_error_flag = 0;
    close_all_switches();
    if ((referee_available()
        && fabsf(power_info.battery_voltage - referee_info.chassis_voltage) > SELF_CHECK_VOLTAGE_DIFF_TOLERANCE) ||
        (referee_available() == 0 && !(power_info.battery_voltage > SELF_CHECK_VOLTAGE_BATTERY_MINUMUN
            && power_info.battery_voltage < SELF_CHECK_VOLTAGE_BATTERY_MAXIMUN)))
        voltage_sensor_check_error_flag++;

    if (step == 10 && voltage_sensor_check_error_flag == 0)
        GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Voltage Sensor Pass");
    else if (step == 10 && voltage_sensor_check_error_flag != 0)
        GUI_TextBoxAppend(textbox, C_DARK_RED, "Voltage Sensor err");

    return voltage_sensor_check_error_flag;
}

unsigned char slefcheck_passthrough_components(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char passthrough_components_check_error_flag = 0;
    switch (step) {
        case 1:pid_set_expect(0);
            close_all_switches();
            break;
        case 4:
            if (power_info.chassis_voltage > SELF_CHECK_VOLTAGE_GND_MAXIMUM) {
                passthrough_components_check_error_flag++;
                GUI_TextBoxAppend(textbox, C_DARK_RED, "pass_sw can't close");
            }
            break;
        case 5:passthrough_switch_only(0);
            break;
        case 8:
            if ((fabsf(power_info.chassis_voltage - power_info.battery_voltage) > SELF_CHECK_VOLTAGE_DIFF_TOLERANCE)) {
                passthrough_components_check_error_flag++;
                GUI_TextBoxAppend(textbox, C_DARK_RED, "pass_sw can't open");
            }
            break;
        case 10:
            if (passthrough_components_check_error_flag == 0)
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Pass Through Pass");
            close_all_switches();
            break;
        default:break;
    }
    return passthrough_components_check_error_flag;
}

unsigned char slefcheck_charge_components(TextBox_Struct_t *textbox, unsigned char step) {
    /* TODO: 此处主要检测充电模块是否正常，检测前先关断所有开关，只对超级电容充电。
     *       主要判断方法是充电时电流传感器是否有正向电流、裁判系统是否有电流输出以及电容电压是否增加 */
    return 0;
}

unsigned char slefcheck_boost_components(TextBox_Struct_t *textbox, unsigned char step) {
    /* TODO: 此处主要检测升压模块是否正常，检测前先关断所有开关，只开启升压
     *       主要判断方法是输出电压应大于26V，以及输出电流传感器应有微小的值 */
    return 0;
}

unsigned char slefcheck_referee_status(TextBox_Struct_t *textbox, unsigned char step) {
    /* TODO: 此处主要检测裁判系统是否连接正常，检测前先关断所有开关，只对超级电容充电。
     *       主要判断方法是裁判系统在线且充电时裁判系统反馈的功率会发生变化。 */
    return 0;
}

unsigned char slefcheck_nuc_status(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char nuc_status_check_error_flag = 0;
    if (!nuc_available())
        nuc_status_check_error_flag++;
    if (step == 10 && nuc_status_check_error_flag == 0)
        GUI_TextBoxAppend(textbox, C_DARK_GREEN, "NUC link check pass");
    else if (step == 10 && nuc_status_check_error_flag != 0)
        GUI_TextBoxAppend(textbox, C_DARK_RED, "NUC link check err");
    return nuc_status_check_error_flag;
}
