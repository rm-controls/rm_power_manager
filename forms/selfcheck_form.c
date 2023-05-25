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
    fsm_set_mode(selftest_all_off_mode);
}

void SelfCheckForm_Update(void) {
    round_counter++;
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
}