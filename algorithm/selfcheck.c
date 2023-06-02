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
            current_sensor_check_error_flag = 0;
            break;
        case 10:
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
        case 20:
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
    if (step == 1)
        voltage_sensor_check_error_flag = 0;
    if ((referee_available()
        && fabsf(power_info.battery_voltage - referee_info.chassis_voltage) > SELF_CHECK_VOLTAGE_DIFF_TOLERANCE) ||
        (referee_available() == 0 && !(power_info.battery_voltage > SELF_CHECK_VOLTAGE_BATTERY_MINUMUN
            && power_info.battery_voltage < SELF_CHECK_VOLTAGE_BATTERY_MAXIMUN)))
        voltage_sensor_check_error_flag++;

    if (step == 20 && voltage_sensor_check_error_flag == 0)
        GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Voltage Sensor Pass");
    else if (step == 20 && voltage_sensor_check_error_flag != 0)
        GUI_TextBoxAppend(textbox, C_DARK_RED, "Voltage Sensor err");

    return voltage_sensor_check_error_flag;
}

unsigned char slefcheck_passthrough_components(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char passthrough_components_check_error_flag = 0;
    switch (step) {
        case 1:pid_set_expect(0);
            close_all_switches();
            passthrough_components_check_error_flag = 0;
            break;
        case 12:
            if (power_info.chassis_voltage > SELF_CHECK_VOLTAGE_GND_MAXIMUM) {
                passthrough_components_check_error_flag++;
                GUI_TextBoxAppend(textbox, C_DARK_RED, "pass_sw can't close");
            }
            passthrough_switch_only(0);
            break;
        case 16:
            if ((fabsf(power_info.chassis_voltage - power_info.battery_voltage) > SELF_CHECK_VOLTAGE_DIFF_TOLERANCE)) {
                passthrough_components_check_error_flag++;
                GUI_TextBoxAppend(textbox, C_DARK_RED, "pass_sw can't open");
            }
            break;
        case 20:
            if (passthrough_components_check_error_flag == 0)
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Pass Through Pass");
            close_all_switches();
            break;
        default:break;
    }
    return passthrough_components_check_error_flag;
}

unsigned char slefcheck_charge_components(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char charge_components_check_error_flag = 0;
    static float last_cap_energy = 0, current_cap_energy = 0;
    pid_calculate_enable_flag = 0;
    dac_set_output((unsigned short) (273.0f * 40.0f / power_info.capacitor_voltage));
    switch (step) {
        case 1:charge_components_check_error_flag = 0, last_cap_energy = 0, current_cap_energy = 0;
            last_cap_energy = 7.5f * power_info.capacitor_voltage * power_info.capacitor_voltage;
            charge_switch_only();
            break;
        case 6:
            if (power_info.charge_current <= 0.1f) {
                charge_components_check_error_flag++;
                sprintf(textbox_line_buffer, "cap_cur nan %.2fA", power_info.charge_current);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
            }
            break;
        case 12:
            if (referee_info.chassis_current <= 0.1f) {
                charge_components_check_error_flag++;
                sprintf(textbox_line_buffer, "ref_cur nan %.2fA", referee_info.chassis_current);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
            }
            break;
        case 18:current_cap_energy = 7.5f * power_info.capacitor_voltage * power_info.capacitor_voltage;
            if ((current_cap_energy - last_cap_energy) < 30.0f) {
                charge_components_check_error_flag++;
                sprintf(textbox_line_buffer, "cap_energy %.1fJ", (current_cap_energy - last_cap_energy));
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
            }
            break;
        case 20:close_all_switches();
            dac_set_output(0);
            if (charge_components_check_error_flag == 0)
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Charge Pass");
            break;
        default:break;
    }

    return charge_components_check_error_flag;
}

unsigned char slefcheck_boost_components(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char boost_components_check_error_flag = 0;
    dac_set_output((unsigned short) (273.0f * 40.0f / power_info.capacitor_voltage));
    switch (step) {
        case 1:close_all_switches();
            dac_set_output(0);
            boost_components_check_error_flag = 0;
            break;
        case 12:
            if (power_info.chassis_voltage > SELF_CHECK_VOLTAGE_GND_MAXIMUM) {
                boost_components_check_error_flag++;
                GUI_TextBoxAppend(textbox, C_DARK_RED, "boost_sw can't close");
            }
            charge_with_boost_switches(0, 0);
            break;
        case 16:
            if (power_info.chassis_voltage >= 23.0f) {
                boost_components_check_error_flag++;
                GUI_TextBoxAppend(textbox, C_DARK_RED, "boost_sw can't open");
            }
            break;
        case 20:
            if (boost_components_check_error_flag == 0)
                GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Boost Component Pass");
            close_all_switches();
            dac_set_output(0);
            break;
        default:break;
    }
    return boost_components_check_error_flag;
}

unsigned char slefcheck_referee_status(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char referee_status_check_error_flag = 0;
    if (step == 1)
        referee_status_check_error_flag = 0;
    if (!referee_available())
        referee_status_check_error_flag++;
    if (step == 20 && referee_status_check_error_flag == 0)
        GUI_TextBoxAppend(textbox, C_DARK_GREEN, "Referee check pass");
    else if (step == 20 && referee_status_check_error_flag != 0)
        GUI_TextBoxAppend(textbox, C_DARK_RED, "Referee link err");
    return referee_status_check_error_flag;
}

unsigned char slefcheck_nuc_status(TextBox_Struct_t *textbox, unsigned char step) {
    static unsigned char nuc_status_check_error_flag = 0;
    if (step == 1)
        nuc_status_check_error_flag = 0;
    if (!nuc_available())
        nuc_status_check_error_flag++;
    if (step == 20 && nuc_status_check_error_flag == 0)
        GUI_TextBoxAppend(textbox, C_DARK_GREEN, "NUC link check pass");
    else if (step == 20 && nuc_status_check_error_flag != 0)
        GUI_TextBoxAppend(textbox, C_DARK_RED, "NUC link check err");
    return nuc_status_check_error_flag;
}
