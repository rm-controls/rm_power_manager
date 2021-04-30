#include "main.h"

Chart_Struct_t Voltage_Chart, Power_Chart;
Curve_Struct_t VCapacitor_Curve, VChassis_Curve, VBaterry_Curve;
Curve_Struct_t PCapacitor_Curve, PChassis_Curve;
Lable_Struct_t VCapacitor_Lable, VChassis_Lable, VBaterry_Lable;
Lable_Struct_t PCapacitor_Lable, PChassis_Lable;
Lable_Struct_t FSM_Mode_Lable, Charge_Mode_Lable, Expect_Power_Lable, Typology_Lable;
Button_Struct_t Settings_Button, Log_Button;

void MainWidget_Init(void) {
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
    FSM_Mode_Lable.Y_Pos = 72;
    FSM_Mode_Lable.Color = C_BLACK;
    FSM_Mode_Lable.Text = "FSM: Normal";

    Charge_Mode_Lable.X_Pos = 2;
    Charge_Mode_Lable.Y_Pos = 86;
    Charge_Mode_Lable.Color = C_BLACK;
    Charge_Mode_Lable.Text = "Charge: Proportional";

    Expect_Power_Lable.X_Pos = 2;
    Expect_Power_Lable.Y_Pos = 100;
    Expect_Power_Lable.Color = C_BLACK;
    Expect_Power_Lable.Text = "Expect Power: 30W";

    Typology_Lable.X_Pos = 2;
    Typology_Lable.Y_Pos = 114;
    Typology_Lable.Color = C_BLACK;
    Typology_Lable.Text = "Typology: Charge&PMOS";

    Log_Button.X_Pos = 2;
    Log_Button.Y_Pos = 134;
    Log_Button.Width = 60;
    Log_Button.Height = 22;
    Log_Button.Text = "Log";

    Settings_Button.X_Pos = 64;
    Settings_Button.Y_Pos = 134;
    Settings_Button.Width = 60;
    Settings_Button.Height = 22;
    Settings_Button.Text = "Settings";

    GUI_Clear(C_WHITE);
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
    GUI_InitLable(&Typology_Lable);
    GUI_InitButton(&Settings_Button);
    GUI_InitButton(&Log_Button);
}
