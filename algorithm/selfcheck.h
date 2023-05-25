//
// Created by LaoZhu on 2023/5/26.
//

#ifndef POWER_MANAGER_ALGORITHM_SELFCHECK_H_
#define POWER_MANAGER_ALGORITHM_SELFCHECK_H_

#define SELF_CHECK_CURRENT_MIDDLE_VOLTAGE   1706
#define SELF_CHECK_CURRENT_TOLERANCE        300

void slefcheck_current_sensor(TextBox_Struct_t *textbox, unsigned char step);
void slefcheck_voltage_sensor(TextBox_Struct_t *textbox, unsigned char step);
void slefcheck_referee_status(TextBox_Struct_t *textbox, unsigned char step);
void slefcheck_nuc_status(TextBox_Struct_t *textbox, unsigned char step);
void slefcheck_passthrough_components(TextBox_Struct_t *textbox, unsigned char step);
void slefcheck_charge_components(TextBox_Struct_t *textbox, unsigned char step);
void slefcheck_boost_components(TextBox_Struct_t *textbox, unsigned char step);

#endif //POWER_MANAGER_ALGORITHM_SELFCHECK_H_
