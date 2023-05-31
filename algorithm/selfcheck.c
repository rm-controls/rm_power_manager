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
        case 1:
            pid_set_expect(0);
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
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Current sensor pass");
            break;
        default:
            break;
    }
    return current_sensor_check_error_flag;
}

unsigned char slefcheck_voltage_sensor(TextBox_Struct_t *textbox, unsigned char step) {
    /* TODO: 此处可直接检测的传感器只有输入电压传感器，可用其值与裁判系统的做对比。若裁判系统离线，则做电压范围判断；
     *       电容电压传感器是否工作正常，无法直接判断（充电时电压不增加有可能是充电模块的问题）；
     *       输出电压传感器是否工作正常，无法直接判断（输出电压不正常有可能是其它模块的问题）*/
    static unsigned char voltage_sensor_check_error_flag = 0;
    static unsigned char nuc_status_check_error_flag = 0;
    if (!referee_available())
        nuc_status_check_error_flag++;
    if (nuc_status_check_error_flag == 0 &&
        fabs(power_info.battery_voltage - referee_info.chassis_voltage) > 0.2f) {
        voltage_sensor_check_error_flag++;
    } else if (nuc_status_check_error_flag != 0 &&
               !(power_info.battery_voltage > 22.0f && power_info.battery_voltage < 26.0f)) {
        voltage_sensor_check_error_flag++;
    }

    if (step == 10 && voltage_sensor_check_error_flag == 0) {
        GUI_TextBoxAppend(textbox, C_DARK_GREEN, "voltage sensor pass");
    } else if (step == 10 && voltage_sensor_check_error_flag != 0) {
        GUI_TextBoxAppend(textbox, C_DARK_RED, "voltage sensor err");
    }

    return voltage_sensor_check_error_flag;
}

unsigned char slefcheck_passthrough_components(TextBox_Struct_t *textbox, unsigned char step) {
    /* TODO: 此处主要检测直通的高侧开关是否能够正常控制开启和关断，检测前先关断所有开关
     *       主要判断方法是关闭高侧开关时输出应无电压，开启时输出电压应与输入电压相差小于一个阈值。 */
    static unsigned char passthrough_components_check_error_flag = 0;
    switch (step) {
        case 1:
            pid_set_expect(0);
            close_all_switches();
            break;
        case 4:
            if (power_info.chassis_voltage > 10.0f) {
                passthrough_components_check_error_flag++;
            }
            break;
        case 5:
            passthrough_switch_only(0);
            break;
        case 7:
            if ((power_info.chassis_voltage - power_info.battery_voltage) > 0.1f) {
            }
            break;
        case 9:
            if (passthrough_components_check_error_flag == 0) {
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "passthrough  pass");
            } else {
                GUI_TextBoxAppend(textbox, C_DARK_RED, "passthrough  err");
            }
            break;
        case 10:
            close_all_switches();
            break;

    }
    return passthrough_components_check_error_flag;
}

unsigned char slefcheck_charge_components(TextBox_Struct_t *textbox, unsigned char step) {
    /* TODO: 此处主要检测充电模块是否正常，检测前先关断所有开关，只对超级电容充电。
     *       主要判断方法是充电时电流传感器是否有正向电流、裁判系统是否有电流输出以及电容电压是否增加 */
    static unsigned char charge_components_check_error_flag;
    static float last_cap_voltage;
    pid_calculate_enable_flag = 0;
    charge_switch_only();
    dac_set_output((unsigned short) (273.0f * 35.0f / power_info.capacitor_voltage));

    switch (step) {
        case 1:
            last_cap_voltage = power_info.capacitor_voltage;
            break;
        case 3:
            if (adc_result[0] <= 0.1) {
                charge_components_check_error_flag++;
            }
            break;
        case 6:
            if (referee_info.chassis_current <= 0.1) {
                charge_components_check_error_flag++;
            }
            break;
        case 9:
            /*充电的电压大于0.3V就算成功*/
            if ((power_info.capacitor_voltage - last_cap_voltage) < 0.3f) {
                charge_components_check_error_flag++;
            }
            break;
        case 10:
            close_all_switches();
            pid_calculate_enable_flag = 1;
            if (charge_components_check_error_flag == 0) {
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "charge  pass");
            } else {
                GUI_TextBoxAppend(textbox, C_DARK_RED, "charge  err");
            }
            break;

    }

    return charge_components_check_error_flag;
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
