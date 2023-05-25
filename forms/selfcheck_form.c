//
// Created by LaoZhu on 2023/5/26.
//

#include "main.h"

static unsigned int round_counter = 0;
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

static unsigned char capacitor_discharge_flag = 0, capacitor_charge_flag = 0;
void SelfCheckForm_Update(void) {
    if (power_info.capacitor_voltage > 13.0f) {
        if (capacitor_discharge_flag == 0)
            GUI_TextBoxAppend(&SelfCheck_TextBox, C_BLACK, "Discharging Cap");
        charge_with_boost_switches(0, 1);
        capacitor_discharge_flag = 1;
        return;
    } else if (power_info.capacitor_voltage < 10.0f) {
        if (capacitor_charge_flag == 0)
            GUI_TextBoxAppend(&SelfCheck_TextBox, C_BLACK, "Charging Cap");
        pid_calculate_enable_flag = 0;
        charge_switch_only();
        dac_set_output((unsigned short) (273.0f * 35.0f / power_info.capacitor_voltage));
        capacitor_charge_flag = 1;
        return;
    }
    round_counter++;
    capacitor_charge_flag = 0;
    capacitor_discharge_flag = 0;
    if (round_counter <= 10)
        slefcheck_current_sensor(&SelfCheck_TextBox, round_counter);
    else if (round_counter <= 20)
        slefcheck_voltage_sensor(&SelfCheck_TextBox, (round_counter - 10));
    else if (round_counter <= 30)
        slefcheck_passthrough_components(&SelfCheck_TextBox, (round_counter - 20));
    else if (round_counter <= 40)
        slefcheck_charge_components(&SelfCheck_TextBox, (round_counter - 30));
    else if (round_counter <= 50)
        slefcheck_boost_components(&SelfCheck_TextBox, (round_counter - 40));
    else if (round_counter <= 60)
        slefcheck_referee_status(&SelfCheck_TextBox, (round_counter - 50));
    else if (round_counter <= 80)
        slefcheck_nuc_status(&SelfCheck_TextBox, (round_counter - 60));
    else {
        fsm_set_mode(normal_mode);
        vTaskResume(fsm_task_handler);
        pid_calculate_enable_flag = 1;
        capacitor_charge_flag = 0;
        capacitor_discharge_flag = 0;
        round_counter = 0;
        Form_Info_Structure.Form_Index = Main_Form_Index;
        MainForm_Init();
    }

    pack_powerinfo_buffer();
}