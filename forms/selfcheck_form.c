//
// Created by LaoZhu on 2023/5/26.
//

#include "main.h"

static TextBox_Struct_t SelfCheck_TextBox;

void SelfCheckForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = NULL;

    SelfCheck_TextBox.X_Pos = 2;
    SelfCheck_TextBox.Y_Pos = 2;
    SelfCheck_TextBox.Height = 134;
    SelfCheck_TextBox.Width = 124;

    gui_clear_screen(C_WHITE);
    GUI_InitTextBox(&SelfCheck_TextBox);
    fsm_set_mode(selftest_mode);
    vTaskSuspend(fsm_task_handler);
}

static unsigned char capacitor_discharge_flag = 0, capacitor_charge_flag = 0, global_error_flag = 0;
static unsigned int capacitor_voltage_control_timeout = 0, round_counter = 0;
void SelfCheckForm_Update(void) {
    if (capacitor_voltage_control_timeout == 400) {
        if (capacitor_discharge_flag == 1)
            GUI_TextBoxAppend(&SelfCheck_TextBox, C_DARK_RED, "Discharging Timeout");
        else if (capacitor_charge_flag == 1)
            GUI_TextBoxAppend(&SelfCheck_TextBox, C_DARK_RED, "Charging Timeout");
    }
    if (capacitor_voltage_control_timeout >= 430)
        goto return_to_main_form;
    if (power_info.capacitor_voltage > 13.0f && capacitor_discharge_flag != 2 && capacitor_charge_flag != 2) {
        if (capacitor_discharge_flag == 0)
            GUI_TextBoxAppend(&SelfCheck_TextBox, C_BLACK, "Discharging Cap");
        charge_with_boost_switches(0, 1);
        capacitor_discharge_flag = 1;
        capacitor_voltage_control_timeout++;
        return;
    } else if (power_info.capacitor_voltage < 10.0f && capacitor_discharge_flag != 2
        && capacitor_charge_flag != 2) {
        if (capacitor_charge_flag == 0)
            GUI_TextBoxAppend(&SelfCheck_TextBox, C_BLACK, "Charging Cap");
        pid_calculate_enable_flag = 0;
        charge_switch_only();
        dac_set_output((unsigned short) (273.0f * 35.0f / power_info.capacitor_voltage));
        capacitor_charge_flag = 1;
        capacitor_voltage_control_timeout++;
        return;
    }
    round_counter++;
    capacitor_discharge_flag = 2;
    capacitor_charge_flag = 2;
    if (round_counter <= 20)
        global_error_flag |= slefcheck_current_sensor(&SelfCheck_TextBox, round_counter);
    else if (round_counter <= 40)
        global_error_flag |= slefcheck_voltage_sensor(&SelfCheck_TextBox, (round_counter - 20));
    else if (round_counter <= 60)
        global_error_flag |= slefcheck_passthrough_components(&SelfCheck_TextBox, (round_counter - 40));
    else if (round_counter <= 80)
        global_error_flag |= slefcheck_charge_components(&SelfCheck_TextBox, (round_counter - 60));
    else if (round_counter <= 100)
        global_error_flag |= slefcheck_boost_components(&SelfCheck_TextBox, (round_counter - 80));
    else if (round_counter <= 120)
        global_error_flag |= slefcheck_referee_status(&SelfCheck_TextBox, (round_counter - 100));
    else if (round_counter <= 160)
        global_error_flag |= slefcheck_nuc_status(&SelfCheck_TextBox, (round_counter - 120));
    else if (global_error_flag == 0) {
        return_to_main_form:
        fsm_set_mode(normal_mode);
        vTaskResume(fsm_task_handler);
        pid_calculate_enable_flag = 1;
        capacitor_charge_flag = 0;
        capacitor_discharge_flag = 0;
        global_error_flag = 0;
        round_counter = 0;
        capacitor_voltage_control_timeout = 0;
        Form_Info_Structure.Form_Index = Main_Form_Index;
        MainForm_Init();
    }

    pack_powerinfo_buffer();
}