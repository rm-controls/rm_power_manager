//
// Created by Lao·Zhu on 2021/4/30.
//

#include "gui_chart.h"
#include "gui_base.h"

void GUI_InitChart(Chart_Struct_t *Chart) {
    Chart->MaxAmplification = 1000;
    gui_draw_rectangle(Chart->X_Pos, Chart->Y_Pos, Chart->Width, Chart->Height, C_WHITE, Filled);
    if (Chart->Coordinate_Style == Rectangular_Coordinate) {
        for (unsigned char counter = 0; counter < 6; counter++)
            gui_draw_point(Chart->X_Pos + counter / 2, Chart->Y_Pos + counter, C_BLACK);
        for (unsigned char counter = 0; counter < 6; counter++)
            gui_draw_point(Chart->X_Pos + Chart->Width - counter,
                           Chart->Y_Pos + Chart->Height - counter / 2,
                           C_BLACK);
        gui_draw_hline(Chart->X_Pos, Chart->Y_Pos + Chart->Height, Chart->Width, C_BLACK);
        gui_draw_vline(Chart->X_Pos, Chart->Y_Pos, Chart->Height, C_BLACK);
    }
}

void GUI_InitCurve(Curve_Struct_t *Curve, Chart_Struct_t *Chart, unsigned short color) {
    for (int counter = 0; counter < sizeof(Curve->Y_Pos); counter++)
        Curve->Y_Pos[counter] = 0x00;
    Curve->Chart = Chart;
    Curve->Color = color;
    Curve->Amplification_Factor = 1000.0f;
    Curve->Pos_Counter = 1;
}

void GUI_CurveAppend(Curve_Struct_t *Curve, float Value) {
    float Current_Amplify;
    if (Value < 1.0f)
        Current_Amplify = (float) (Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Chart->Y_Pos);
    else if (Value < 10.0f)
        Current_Amplify =
            (float) (Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Chart->Y_Pos) / (Value + 1.0f);
    else if (Value < 100.0f)
        Current_Amplify =
            (float) (Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Chart->Y_Pos) / (Value + 10.0f);
    else if (Value < 1000.0f)
        Current_Amplify =
            (float) (Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Chart->Y_Pos) / (Value + 100.0f);
    else if (Value < 10000.0f)
        Current_Amplify =
            (float) (Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Chart->Y_Pos) / (Value + 1000.0f);
    else
        Current_Amplify =
            (float) (Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Chart->Y_Pos) / (Value + 10000.0f);

    if (Current_Amplify < Curve->Chart->MaxAmplification)
        Curve->Chart->MaxAmplification = Current_Amplify;
    else
        Current_Amplify = Curve->Chart->MaxAmplification;

    if (Current_Amplify < Curve->Amplification_Factor) {
        float Alpha = Current_Amplify / Curve->Amplification_Factor;
        Curve->Amplification_Factor = Current_Amplify;
        for (unsigned char counter = 1; counter < Curve->Pos_Counter; counter++)
            gui_draw_point(counter + Curve->Chart->X_Pos,
                           Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Y_Pos[counter],
                           C_WHITE);
        for (unsigned char counter = 1; counter < Curve->Pos_Counter; counter++) {
            Curve->Y_Pos[counter] = (unsigned char) (Alpha * (float) Curve->Y_Pos[counter]);
            gui_draw_point(counter + Curve->Chart->X_Pos,
                           Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Y_Pos[counter],
                           Curve->Color);
        }
    } else
        Current_Amplify = Curve->Amplification_Factor;

    if (Current_Amplify < Curve->Chart->MaxAmplification)
        Curve->Chart->MaxAmplification = Current_Amplify;
    else
        Current_Amplify = Curve->Chart->MaxAmplification;

    Curve->Y_Pos[Curve->Pos_Counter] = (unsigned char) (Value * Current_Amplify);
    gui_draw_point(Curve->Pos_Counter + Curve->Chart->X_Pos,
                   Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Y_Pos[Curve->Pos_Counter],
                   Curve->Color);
    if (Curve->Chart->Value_Style == Display_8x16_Value)
        gui_printf(Curve->Chart->X_Pos + 3, Curve->Chart->Y_Pos + 2, Curve->Color, C_WHITE, "%.2f", Value);
    if (Curve->Pos_Counter < (Curve->Chart->Width - 3))
        Curve->Pos_Counter++;
    else {
        for (unsigned char counter = 1; counter < (Curve->Pos_Counter + 1); counter++)
            gui_draw_point(counter + Curve->Chart->X_Pos,
                           Curve->Chart->Y_Pos + Curve->Chart->Height - Curve->Y_Pos[counter],
                           C_WHITE);
        if (Curve->Chart->Coordinate_Style == Rectangular_Coordinate) {
            for (unsigned char counter = 0; counter < 6; counter++)
                gui_draw_point(Curve->Chart->X_Pos + counter / 2, Curve->Chart->Y_Pos + counter, C_BLACK);
            for (unsigned char counter = 0; counter < 6; counter++)
                gui_draw_point(Curve->Chart->X_Pos + Curve->Chart->Width - counter,
                               Curve->Chart->Y_Pos + Curve->Chart->Height - counter / 2,
                               C_BLACK);
            gui_draw_hline(Curve->Chart->X_Pos,
                           Curve->Chart->Y_Pos + Curve->Chart->Height,
                           Curve->Chart->Width,
                           C_BLACK);
            gui_draw_vline(Curve->Chart->X_Pos, Curve->Chart->Y_Pos, Curve->Chart->Height, C_BLACK);
        }
        Curve->Amplification_Factor = 1000.0f;
        Curve->Chart->MaxAmplification = 1000.0f;
        Curve->Pos_Counter = 1;
    }
}
