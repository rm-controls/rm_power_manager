//
// Created by Lao·Zhu on 2021/4/30.
//

#include "main.h"

Chart_Struct_t Voltage_Chart, Power_Chart;
Curve_Struct_t VCapacitor_Curve, VChassis_Curve, VBaterry_Curve;
Curve_Struct_t PCapacitor_Curve, PChassis_Curve;
Lable_Struct_t VCapacitor_Lable, VChassis_Lable, VBaterry_Lable;
Lable_Struct_t PCapacitor_Lable, PChassis_Lable;
Lable_Struct_t FSM_Mode_Lable, Charge_Mode_Lable, Expect_Power_Lable,
        Typology_Lable;
Button_Struct_t Settings_Button, Log_Button;

extern uint8_t debug_flag;

void Log_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Log_Form_Index;
    LogForm_Init();
}

void Settings_Button_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Settings_Form_Index;
    if (debug_flag == 0)
        debug_flag = 1;
    SettingsForm_Init();
}

extern unsigned char fff;

void MainForm_Update(void) {
    GUI_CurveAppend(&VCapacitor_Curve, V_Capacitor);
    GUI_CurveAppend(&VChassis_Curve, V_Chassis);
    GUI_CurveAppend(&VBaterry_Curve, V_Baterry);
    GUI_CurveAppend(&PCapacitor_Curve, P_Capacitor);
    GUI_CurveAppend(&PChassis_Curve, P_Chassis);

    GUI_LableSetText(&VCapacitor_Lable, "%2d.%1dV", (int) V_Capacitor,
                     (int) (V_Capacitor * 10) - ((int) V_Capacitor * 10));
    GUI_LableSetText(&VChassis_Lable, "%2d.%1dV", (int) V_Chassis,
                     (int) (V_Chassis * 10) - ((int) V_Chassis * 10));
    GUI_LableSetText(&VBaterry_Lable, "%2d.%1dV", (int) V_Baterry,
                     (int) (V_Baterry * 10) - ((int) V_Baterry * 10));
    GUI_LableSetText(&PCapacitor_Lable, "%3d.%1dW", (int) P_Capacitor,
                     (int) (P_Capacitor * 10) - ((int) P_Capacitor * 10));
    if (fff == HAL_UART_ERROR_ORE) {
        GUI_LableSetText(&PChassis_Lable, "%3d.%1dF", (int) P_Chassis,
                         (int) (P_Chassis * 10) - ((int) P_Chassis * 10));
    } else {
        GUI_LableSetText(&PChassis_Lable, "%3d.%1dW", (int) P_Chassis,
                         (int) (P_Chassis * 10) - ((int) P_Chassis * 10));
    }

    switch (FSM_Status.FSM_Mode) {
        case Normal_Mode:
            GUI_LableSetText(&FSM_Mode_Lable, " FSM Status: Normal  ");
            break;
        case OverPower_Mode:
            GUI_LableSetText(&FSM_Mode_Lable, "FSM Status: OverPower");
            break;
        case Halt_Mode:
            if (overcurrent_flag == 1)
                GUI_LableSetText(&FSM_Mode_Lable, " FSM Status:Halt OVC ");
            else if (Setting_OptiSchemes == SucapTest_Optimized)
                GUI_LableSetText(&FSM_Mode_Lable, "FSM Status:Halt SuCap");
            else
                GUI_LableSetText(&FSM_Mode_Lable, " FSM Status:Halt DRP ");
            break;
        case NoCharge_Mode:
            GUI_LableSetText(&FSM_Mode_Lable, "FSM Status: OnlyChass");
            break;
        case SucapTest_Mode:
            GUI_LableSetText(&FSM_Mode_Lable, "FSM Status:SuCap Test");
            break;
    }
    switch (FSM_Status.Charge_Mode) {
        case Zero_Power_Charge:
            GUI_LableSetText(&Charge_Mode_Lable, "  Charge: ZeroPower  ");
            break;
        case Const_Power_Charge:
            GUI_LableSetText(&Charge_Mode_Lable, " Charge: ConstPower  ");
            break;
        case Proportional_Charge:
            GUI_LableSetText(&Charge_Mode_Lable, "Charge: Proportional ");
            break;
        case Full_Power_Charge:
            GUI_LableSetText(&Charge_Mode_Lable, "  Charge: FullPower  ");
            break;
        case Remain_Power_Charge:
            GUI_LableSetText(&Charge_Mode_Lable, " Charge: RemainPower ");
            break;
    }
    GUI_LableSetText(&Expect_Power_Lable, " Expect Power: %3dW ",
                     (int) EP_Chassis);
    switch (FSM_Status.Typology_Mode) {
        case Boost_With_Charge:
            GUI_LableSetText(&Typology_Lable, "Typology: ChargeBoost");
            break;
        case Only_DeliverChassis:
            GUI_LableSetText(&Typology_Lable, "Typology: OnlyChassis");
            break;
        case Chassis_With_Charge:
            GUI_LableSetText(&Typology_Lable, "Typology: PMOS&Charge");
            break;
        case Only_Charge:
            GUI_LableSetText(&Typology_Lable, "Typology: Only Charge");
            break;
        case All_Off:
            GUI_LableSetText(&Typology_Lable, "  Typology: All Off  ");
            break;
    }
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
    FSM_Mode_Lable.Text = " FSM Status: Normal  ";

    Charge_Mode_Lable.X_Pos = 2;
    Charge_Mode_Lable.Y_Pos = 88;
    Charge_Mode_Lable.Color = C_BLACK;
    Charge_Mode_Lable.Text = "Charge: Proportional ";

    Expect_Power_Lable.X_Pos = 2;
    Expect_Power_Lable.Y_Pos = 102;
    Expect_Power_Lable.Color = C_BLACK;
    Expect_Power_Lable.Text = "  Expect Power: 30W  ";

    Typology_Lable.X_Pos = 2;
    Typology_Lable.Y_Pos = 116;
    Typology_Lable.Color = C_BLACK;
    Typology_Lable.Text = "Typology: Charge&PMOS";

    Log_Button.X_Pos = 2;
    Log_Button.Y_Pos = 134;
    Log_Button.Width = 60;
    Log_Button.Height = 22;
    Log_Button.Text = "Log&Info";
    Log_Button.NextButton = &Settings_Button;
    Log_Button.CallbackFunction = Log_Button_Callback;

    Settings_Button.X_Pos = 64;
    Settings_Button.Y_Pos = 134;
    Settings_Button.Width = 60;
    Settings_Button.Height = 22;
    //    Settings_Button.Text = "Settings";
    Settings_Button.Text = "Debug";
    Settings_Button.NextButton = NULL;
    Settings_Button.CallbackFunction = Settings_Button_Callback;

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
