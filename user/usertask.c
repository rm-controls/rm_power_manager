//
// Created by Lao·Zhu on 2021/2/4.
//

#include "main.h"

void UserTask(void *pvParameters) {
    FSM_Status.uExpect_Mode = Remain_Power_Expect;
    FSM_Status.uCharge_Mode = Const_Power_Charge;
    FSM_Status.FSM_Mode = Normal_Mode;
    FSM_Status.Charge_Power = 20;
    FSM_Status.Max_Power = 100;

    while (1) {
        if (FSM_Status.FSM_Mode != Halt_Mode) {
            if (V_Capacitor >= 15.0f) {         //Enter Over Power Mode
                FSM_Status.FSM_Mode = OverPower_Mode;
//                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
            } else if (V_Capacitor <= 8.0f && V_Capacitor > 7.0f &&
                FSM_Status.FSM_Mode == OverPower_Mode) {
                FSM_Status.FSM_Mode = Transition_Mode;
            } else if (V_Capacitor <= 7.0f) {   //Exit OverPower Mode
                FSM_Status.FSM_Mode = Normal_Mode;
//                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
            }
        }
        Delayms(1);
    }
}

void LCD_Refresh(void *pvParameters) {
    Chart_Struct_t Voltage_Chart;
    Curve_Struct_t VCapacitor_Curve, VChassis_Curve;
    Key_Num_e key_num_old = 0;
    GUI_Init();

    Voltage_Chart.Value_Style = No_Value_Display;
    Voltage_Chart.Height = 80;
    Voltage_Chart.Width = 124;
    Voltage_Chart.Coordinate_Style = Rectangular_Coordinate;
    Voltage_Chart.Start_X = 2;
    Voltage_Chart.Start_Y = 2;
    GUI_InitChart(&Voltage_Chart);

    VCapacitor_Curve.Chart = &Voltage_Chart;
    VCapacitor_Curve.Amplification_Factor = 1000.0f;
    VCapacitor_Curve.Color = C_DARK_RED;
    VCapacitor_Curve.Pos_Counter = 1;

    VChassis_Curve.Chart = &Voltage_Chart;
    VChassis_Curve.Amplification_Factor = 1000.0f;
    VChassis_Curve.Color = C_DARK_BLUE;
    VChassis_Curve.Pos_Counter = 1;
//    unsigned char address = 0;
//    for (address = 0; address < 0xff; address++) {
//        IIC_Start();
//        if (IIC_SendByte(address) == 0)
//            break;
//        IIC_Stop();
//        Delayms(1);
//    }
//    GUI_Printf(0, 0, C_BLACK, C_WHITE, "0x%x", address);
    GUI_Button(20, 90, 88, 40, (unsigned char *) "Hello", Button_Normal);
    Delayms(1000);
    GUI_Button(20, 90, 88, 40, (unsigned char *) "Hello", Button_Focus);
    while (1) {
        Key_Num_e key_num = ReadKey();
        if (key_num_old != key_num) {
            if (key_num == Center_Key)
                GUI_Button(20, 90, 88, 40, (unsigned char *) "Hello", Button_Click);
            else
                GUI_Button(20, 90, 88, 40, (unsigned char *) "Hello", Button_Focus);
            key_num_old = key_num;
        }
        GUI_CurveAppend(&VChassis_Curve, V_Chassis);
        GUI_CurveAppend(&VCapacitor_Curve, V_Capacitor);
        Delayms(100);
    }
}
