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
                sprintf(textbox_line_buffer, "pass_sw close %.1fV", power_info.chassis_voltage);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
            }
            passthrough_switch_only(0);
            break;
        case 16:
            if ((fabsf(power_info.chassis_voltage - power_info.battery_voltage) > SELF_CHECK_VOLTAGE_DIFF_TOLERANCE)) {
                passthrough_components_check_error_flag++;
                sprintf(textbox_line_buffer, "pass_sw open %.1fV", power_info.chassis_voltage);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
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
    dac_set_output((unsigned short) (273.0f * SELF_CHECK_CHARGE_POWER / power_info.capacitor_voltage));
    switch (step) {
        case 1:charge_components_check_error_flag = 0, last_cap_energy = 0, current_cap_energy = 0;
            last_cap_energy = 7.5f * power_info.capacitor_voltage * power_info.capacitor_voltage;
            charge_switch_only();
            break;
        case 6:
            if (power_info.charge_current <= SELF_CHECK_CHARGE_CURRENT_MINIMUM) {
                charge_components_check_error_flag++;
                sprintf(textbox_line_buffer, "cap_cur nan %.2fA", power_info.charge_current);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
            }
            break;
        case 12:
            if (referee_info.chassis_current <= SELF_CHECK_CHARGE_CURRENT_MINIMUM) {
                charge_components_check_error_flag++;
                sprintf(textbox_line_buffer, "ref_cur nan %.2fA", referee_info.chassis_current);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
            }
            break;
        case 18:current_cap_energy = 7.5f * power_info.capacitor_voltage * power_info.capacitor_voltage;
            if ((current_cap_energy - last_cap_energy) < SELF_CHECK_CHARGE_ENERGY_MINIMUM) {
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
    if (step >= 12)
        dac_set_output((unsigned short) (273.0f * SELF_CHECK_CHARGE_POWER / power_info.capacitor_voltage));
    switch (step) {
        case 1:close_all_switches();
            dac_set_output(0);
            boost_components_check_error_flag = 0;
            break;
        case 12:
            if (power_info.chassis_voltage > SELF_CHECK_VOLTAGE_GND_MAXIMUM) {
                boost_components_check_error_flag++;
                sprintf(textbox_line_buffer, "bst_sw close %.1fV", power_info.chassis_voltage);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
            }
            charge_with_boost_switches(0, 0);
            break;
        case 19:
            if (power_info.chassis_voltage < SELF_CHECK_VOLTAGE_BOOST_MINIMUM) {
                boost_components_check_error_flag++;
                sprintf(textbox_line_buffer, "bst_sw open %.1fV", power_info.chassis_voltage);
                GUI_TextBoxAppend(textbox, C_DARK_RED, textbox_line_buffer);
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
    if (step == 20 && nuc_status_check_error_flag < 10) {
        GUI_TextBoxAppend(textbox, C_DARK_GREEN, "NUC link check pass");
        return 0;
    } else if (step == 20 && nuc_status_check_error_flag >= 10) {
        GUI_TextBoxAppend(textbox, C_DARK_RED, "NUC link check err");
        return 1;
    }
    return 0;
}

static unsigned char capacitor_discharge_flag = 0, capacitor_charge_flag = 0;
static unsigned int capacitor_voltage_control_timeout = 0, round_counter = 0;
unsigned short selfcheck_digital_tube(void) {
    if (capacitor_voltage_control_timeout == 400) {
        if (capacitor_discharge_flag == 1)
            GUI_TextBoxAppend(NULL, C_DARK_RED, "Discharging Timeout");
        else if (capacitor_charge_flag == 1)
            GUI_TextBoxAppend(NULL, C_DARK_RED, "Charging Timeout");
        return 0x0A05;
    }
    if (capacitor_voltage_control_timeout >= 430)
        goto resume_normal_mode;
    if (power_info.capacitor_voltage > 13.0f && capacitor_discharge_flag != 2 && capacitor_charge_flag != 2) {
        if (capacitor_discharge_flag == 0)
            GUI_TextBoxAppend(NULL, C_BLACK, "Discharging Cap");
        charge_with_boost_switches(0, 1);
        capacitor_discharge_flag = 1;
        capacitor_voltage_control_timeout++;
        return 0x0F0F;
    } else if (power_info.capacitor_voltage < 10.0f && capacitor_discharge_flag != 2
        && capacitor_charge_flag != 2) {
        if (capacitor_charge_flag == 0)
            GUI_TextBoxAppend(NULL, C_BLACK, "Charging Cap");
        pid_calculate_enable_flag = 0;
        charge_switch_only();
        dac_set_output((unsigned short) (273.0f * 35.0f / power_info.capacitor_voltage));
        capacitor_charge_flag = 1;
        capacitor_voltage_control_timeout++;
        return 0x0F0E;
    }
    round_counter++;
    capacitor_discharge_flag = 2;
    capacitor_charge_flag = 2;
    capacitor_voltage_control_timeout = 0;
    unsigned char error_flag;
    if (round_counter <= 20)
        error_flag = slefcheck_current_sensor(NULL, round_counter);
    else if (round_counter <= 40)
        error_flag = slefcheck_voltage_sensor(NULL, (round_counter - 20));
    else if (round_counter <= 60)
        error_flag = slefcheck_passthrough_components(NULL, (round_counter - 40));
    else if (round_counter <= 80)
        error_flag = slefcheck_charge_components(NULL, (round_counter - 60));
    else if (round_counter <= 100)
        error_flag = slefcheck_boost_components(NULL, (round_counter - 80));
    else if (round_counter <= 120)
        error_flag = slefcheck_referee_status(NULL, (round_counter - 100));
    else if (round_counter <= 140)
        error_flag = slefcheck_nuc_status(NULL, (round_counter - 120));
    else {
        resume_normal_mode:
        fsm_set_mode(normal_mode);
        vTaskResume(fsm_task_handler);
        pid_calculate_enable_flag = 1;
        capacitor_charge_flag = 0;
        capacitor_discharge_flag = 0;
        round_counter = 0;
        capacitor_voltage_control_timeout = 0;
        return 0x0000;
    }

    pack_powerinfo_buffer();
    return ((unsigned short) error_flag << 8UL) | ((round_counter / 20) + 1);
}