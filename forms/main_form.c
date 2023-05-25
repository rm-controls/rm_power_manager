//
// Created by Lao·Zhu on 2021/4/30.
//

#include "main.h"

static Chart_Struct_t Voltage_Chart, Power_Chart;
static Curve_Struct_t VCapacitor_Curve, VChassis_Curve, VBaterry_Curve;
static Curve_Struct_t PCapacitor_Curve, PChassis_Curve;
static Lable_Struct_t VCapacitor_Lable, VChassis_Lable, VBaterry_Lable;
static Lable_Struct_t PCapacitor_Lable, PChassis_Lable;
static Lable_Struct_t FSM_Mode_Lable, Charge_Mode_Lable, Expect_Power_Lable;
static Button_Struct_t Settings_Button, Log_Button, Self_Check_Button, Off_LCD_Button;

static void Self_Check_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = SelfCheck_Form_Index;
    SelfCheckForm_Init();
}

static void Off_LCD_Button_Callback(void *object, unsigned char key) {
    lcd_off_flag = 1;
    HAL_GPIO_WritePin(LCD_PWR_Port, LCD_PWR_Pin, GPIO_PIN_SET);
}

static void Log_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Log_Form_Index;
    LogForm_Init();
}

static void Settings_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Settings_Form_Index;
    SettingsForm_Init();
}

void MainForm_Update(void) {
    GUI_CurveAppend(&VCapacitor_Curve, power_info.capacitor_voltage);
    GUI_CurveAppend(&VChassis_Curve, power_info.chassis_voltage);
    GUI_CurveAppend(&VBaterry_Curve, power_info.battery_voltage);
    GUI_CurveAppend(&PCapacitor_Curve, power_info.charge_power);
    GUI_CurveAppend(&PChassis_Curve, power_info.chassis_power);

    GUI_LableSetText(&VCapacitor_Lable, "%2d.%1dV", (int) power_info.capacitor_voltage,
                     (int) (power_info.capacitor_voltage * 10) - ((int) power_info.capacitor_voltage * 10));
    GUI_LableSetText(&VChassis_Lable, "%2d.%1dV", (int) power_info.chassis_voltage,
                     (int) (power_info.chassis_voltage * 10) - ((int) power_info.chassis_voltage * 10));
    GUI_LableSetText(&VBaterry_Lable, "%2d.%1dV", (int) power_info.battery_voltage,
                     (int) (power_info.battery_voltage * 10) - ((int) power_info.battery_voltage * 10));
    GUI_LableSetText(&PCapacitor_Lable, "%3d.%1dW", (int) power_info.charge_power,
                     (int) (power_info.charge_power * 10) - ((int) power_info.charge_power * 10));
    GUI_LableSetText(&PChassis_Lable, "%3d.%1dW", (int) power_info.chassis_power,
                     (int) (power_info.chassis_power * 10) - ((int) power_info.chassis_power * 10));

    switch (main_fsm.mode) {
        case charge_mode:GUI_LableSetText(&FSM_Mode_Lable, "  FSM Mode: Charge   ");
            break;
        case boost_mode:GUI_LableSetText(&FSM_Mode_Lable, "   FSM Mode: Boost   ");
            break;
        case normal_mode:GUI_LableSetText(&FSM_Mode_Lable, "  FSM Mode: Normal   ");
            break;
        case all_off_mode:GUI_LableSetText(&FSM_Mode_Lable, "  FSM Mode: All off  ");
            break;
        default:break;
    }

    switch (main_fsm.typology) {
        case pass_through:GUI_LableSetText(&Charge_Mode_Lable, "Typology: Passthrough");
            break;
        case charge_with_boost:GUI_LableSetText(&Charge_Mode_Lable, "Typology: ChargeBoost");
            break;
        case switches_all_off:GUI_LableSetText(&Charge_Mode_Lable, "  Typology: All off  ");
            break;
        default:break;
    }

    GUI_LableSetText(&Expect_Power_Lable, " Expect Power: %3dW ", (int) power_info.expect_chassis_power);
}

void MainForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &Log_Button;

    Voltage_Chart.X_Pos = 2;
    Voltage_Chart.Y_Pos = 2;
    Voltage_Chart.Height = 40;
    Voltage_Chart.Width = 60;
    Voltage_Chart.Coordinate_Style = Rectangular_Coordinate;
    Voltage_Chart.Value_Style = No_Value_Display;

    Power_Chart.X_Pos = 66;
    Power_Chart.Y_Pos = 2;
    Power_Chart.Height = 40;
    Power_Chart.Width = 60;
    Power_Chart.Coordinate_Style = Rectangular_Coordinate;
    Power_Chart.Value_Style = No_Value_Display;

    VCapacitor_Lable.X_Pos = 4;
    VCapacitor_Lable.Y_Pos = 44;
    VCapacitor_Lable.Color = C_DARK_RED;
    VCapacitor_Lable.Text = "--.-V";

    VChassis_Lable.X_Pos = 49;
    VChassis_Lable.Y_Pos = 44;
    VChassis_Lable.Color = C_DARK_ORANGE;
    VChassis_Lable.Text = "--.-V";

    VBaterry_Lable.X_Pos = 94;
    VBaterry_Lable.Y_Pos = 44;
    VBaterry_Lable.Color = C_DARK_GOLDEN_ROD;
    VBaterry_Lable.Text = "--.-V";

    PCapacitor_Lable.X_Pos = 18;
    PCapacitor_Lable.Y_Pos = 58;
    PCapacitor_Lable.Color = C_DARK_BLUE;
    PCapacitor_Lable.Text = "---.-W";

    PChassis_Lable.X_Pos = 72;
    PChassis_Lable.Y_Pos = 58;
    PChassis_Lable.Color = C_DARK_GREEN;
    PChassis_Lable.Text = "---.-W";

    FSM_Mode_Lable.X_Pos = 2;
    FSM_Mode_Lable.Y_Pos = 74;
    FSM_Mode_Lable.Color = C_BLACK;
    FSM_Mode_Lable.Text = "  FSM Mode: Normal   ";

    Charge_Mode_Lable.X_Pos = 2;
    Charge_Mode_Lable.Y_Pos = 88;
    Charge_Mode_Lable.Color = C_BLACK;
    Charge_Mode_Lable.Text = "Typology: Passthrough";

    Expect_Power_Lable.X_Pos = 2;
    Expect_Power_Lable.Y_Pos = 102;
    Expect_Power_Lable.Color = C_BLACK;
    Expect_Power_Lable.Text = "  Expect Power: 30W  ";

    Log_Button.X_Pos = 2;
    Log_Button.Y_Pos = 114;
    Log_Button.Width = 60;
    Log_Button.Height = 20;
    Log_Button.Text = "Log&Info";
    Log_Button.NextButton = &Settings_Button;
    Log_Button.CallbackFunction = Log_Button_Callback;

    Settings_Button.X_Pos = 64;
    Settings_Button.Y_Pos = 114;
    Settings_Button.Width = 60;
    Settings_Button.Height = 20;
    Settings_Button.Text = "Settings";
    Settings_Button.NextButton = &Self_Check_Button;
    Settings_Button.CallbackFunction = Settings_Button_Callback;

    Self_Check_Button.X_Pos = 2;
    Self_Check_Button.Y_Pos = 136;
    Self_Check_Button.Width = 60;
    Self_Check_Button.Height = 20;
    Self_Check_Button.Text = "SelfCheck";
    Self_Check_Button.NextButton = &Off_LCD_Button;
    Self_Check_Button.CallbackFunction = Self_Check_Button_Callback;

    Off_LCD_Button.X_Pos = 64;
    Off_LCD_Button.Y_Pos = 136;
    Off_LCD_Button.Width = 60;
    Off_LCD_Button.Height = 20;
    Off_LCD_Button.Text = "Off LCD";
    Off_LCD_Button.NextButton = NULL;
    Off_LCD_Button.CallbackFunction = Off_LCD_Button_Callback;

    gui_clear_screen(C_WHITE);
    GUI_InitCurve(&VCapacitor_Curve, &Voltage_Chart, VCapacitor_Lable.Color);
    GUI_InitCurve(&VChassis_Curve, &Voltage_Chart, VChassis_Lable.Color);
    GUI_InitCurve(&VBaterry_Curve, &Voltage_Chart, VBaterry_Lable.Color);
    GUI_InitCurve(&PCapacitor_Curve, &Power_Chart, PCapacitor_Lable.Color);
    GUI_InitCurve(&PChassis_Curve, &Power_Chart, PChassis_Lable.Color);
    GUI_InitChart(&Voltage_Chart);
    GUI_InitChart(&Power_Chart);
    GUI_InitLable(&VCapacitor_Lable);
    GUI_InitLable(&VChassis_Lable);
    GUI_InitLable(&VBaterry_Lable);
    GUI_InitLable(&PCapacitor_Lable);
    GUI_InitLable(&PChassis_Lable);
    GUI_InitLable(&FSM_Mode_Lable);
    GUI_InitLable(&Charge_Mode_Lable);
    GUI_InitLable(&Expect_Power_Lable);
    GUI_InitButton(&Settings_Button);
    GUI_InitButton(&Log_Button);
    GUI_InitButton(&Self_Check_Button);
    GUI_InitButton(&Off_LCD_Button);
}
