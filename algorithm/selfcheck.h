//
// Created by LaoZhu on 2023/5/26.
//

#ifndef POWER_MANAGER_ALGORITHM_SELFCHECK_H_
#define POWER_MANAGER_ALGORITHM_SELFCHECK_H_

#define SELF_CHECK_CURRENT_MIDDLE_VOLTAGE   100
#define SELF_CHECK_CURRENT_TOLERANCE        100
#define SELF_CHECK_VOLTAGE_DIFF_TOLERANCE   0.5f
#define SELF_CHECK_VOLTAGE_GND_MAXIMUM      5.0f
#define SELF_CHECK_VOLTAGE_BATTERY_MINUMUN  21.0f
#define SELF_CHECK_VOLTAGE_BATTERY_MAXIMUN  27.0f
#define SELF_CHECK_VOLTAGE_BOOST_MINIMUM    23.5f
#define SELF_CHECK_CHARGE_CURRENT_MINIMUM   0.5f
#define SELF_CHECK_CHARGE_ENERGY_MINIMUM    30.0f
#define SELF_CHECK_CHARGE_POWER             40.0f

unsigned char slefcheck_current_sensor(TextBox_Struct_t *textbox, unsigned char step);
unsigned char slefcheck_voltage_sensor(TextBox_Struct_t *textbox, unsigned char step);
unsigned char slefcheck_referee_status(TextBox_Struct_t *textbox, unsigned char step);
unsigned char slefcheck_nuc_status(TextBox_Struct_t *textbox, unsigned char step);
unsigned char slefcheck_passthrough_components(TextBox_Struct_t *textbox, unsigned char step);
unsigned char slefcheck_charge_components(TextBox_Struct_t *textbox, unsigned char step);
unsigned char slefcheck_boost_components(TextBox_Struct_t *textbox, unsigned char step);
unsigned short selfcheck_digital_tube(void);

#endif  // POWER_MANAGER_ALGORITHM_SELFCHECK_H_
