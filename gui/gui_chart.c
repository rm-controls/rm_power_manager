//
// Created by Lao·Zhu on 2021/4/30.
//

#include "gui_chart.h"
#include "gui_base.h"

void GUI_InitChart(Chart_Struct_t *Chart) {
    Chart->MaxAmplification = 1000;
    GUI_DrawRectangle(Chart->Start_X, Chart->Start_Y, Chart->Width, Chart->Height, C_WHITE, Filled);
    if (Chart->Coordinate_Style == Rectangular_Coordinate) {
        for (unsigned char counter = 0; counter < 6; counter++)
            GUI_Point(Chart->Start_X + counter / 2, Chart->Start_Y + counter, C_BLACK);
        for (unsigned char counter = 0; counter < 6; counter++)
            GUI_Point(Chart->Start_X + Chart->Width - counter,
                      Chart->Start_Y + Chart->Height - counter / 2,
                      C_BLACK);
        GUI_DrawHLine(Chart->Start_X, Chart->Start_Y + Chart->Height, Chart->Width, C_BLACK);
        GUI_DrawVLine(Chart->Start_X, Chart->Start_Y, Chart->Height, C_BLACK);
    }
}

void GUI_CurveAppend(Curve_Struct_t *Curve, float Value) {
    float Current_Amplify;
    if (Value < 1.0f)
        Current_Amplify = (float) (Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Chart->Start_Y);
    else if (Value < 10.0f)
        Current_Amplify =
            (float) (Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Chart->Start_Y) / (Value + 1.0f);
    else if (Value < 100.0f)
        Current_Amplify =
            (float) (Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Chart->Start_Y) / (Value + 10.0f);
    else if (Value < 1000.0f)
        Current_Amplify =
            (float) (Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Chart->Start_Y) / (Value + 100.0f);
    else if (Value < 10000.0f)
        Current_Amplify =
            (float) (Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Chart->Start_Y) / (Value + 1000.0f);
    else
        Current_Amplify =
            (float) (Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Chart->Start_Y) / (Value + 10000.0f);
    if (Current_Amplify < Curve->Amplification_Factor) {
        float Alpha = Current_Amplify / Curve->Amplification_Factor;
        Curve->Amplification_Factor = Current_Amplify;
        for (unsigned char counter = 1; counter < Curve->Pos_Counter; counter++)
            GUI_Point(counter + Curve->Chart->Start_X,
                      Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Y_Pos[counter],
                      C_WHITE);
        for (unsigned char counter = 1; counter < Curve->Pos_Counter; counter++) {
            Curve->Y_Pos[counter] = (unsigned char) (Alpha * (float) Curve->Y_Pos[counter]);
            GUI_Point(counter + Curve->Chart->Start_X,
                      Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Y_Pos[counter],
                      Curve->Color);
        }
    } else
        Current_Amplify = Curve->Amplification_Factor;
    if (Current_Amplify < Curve->Chart->MaxAmplification)
        Curve->Chart->MaxAmplification = Current_Amplify;
    else
        Current_Amplify = Curve->Chart->MaxAmplification;
    Curve->Y_Pos[Curve->Pos_Counter] = (unsigned char) (Value * Current_Amplify);
    GUI_Point(Curve->Pos_Counter + Curve->Chart->Start_X,
              Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Y_Pos[Curve->Pos_Counter],
              Curve->Color);
    if (Curve->Chart->Value_Style == Display_8x16_Value)
        GUI_Printf(Curve->Chart->Start_X + 3, Curve->Chart->Start_Y + 2, Curve->Color, C_WHITE, "%.2f", Value);
    if (Curve->Pos_Counter < 124)
        Curve->Pos_Counter++;
    else {
        for (unsigned char counter = 1; counter < Curve->Pos_Counter; counter++)
            GUI_Point(counter + Curve->Chart->Start_X,
                      Curve->Chart->Start_Y + Curve->Chart->Height - Curve->Y_Pos[counter],
                      C_WHITE);
        if (Curve->Chart->Coordinate_Style == Rectangular_Coordinate) {
            for (unsigned char counter = 0; counter < 6; counter++)
                GUI_Point(Curve->Chart->Start_X + counter / 2, Curve->Chart->Start_Y + counter, C_BLACK);
            for (unsigned char counter = 0; counter < 6; counter++)
                GUI_Point(Curve->Chart->Start_X + Curve->Chart->Width - counter,
                          Curve->Chart->Start_Y + Curve->Chart->Height - counter / 2,
                          C_BLACK);
            GUI_DrawHLine(Curve->Chart->Start_X,
                          Curve->Chart->Start_Y + Curve->Chart->Height,
                          Curve->Chart->Width,
                          C_BLACK);
            GUI_DrawVLine(Curve->Chart->Start_X, Curve->Chart->Start_Y, Curve->Chart->Height, C_BLACK);
        }
        Curve->Amplification_Factor = 1000.0f;
        Curve->Chart->MaxAmplification = 1000.0f;
        Curve->Pos_Counter = 1;
    }
}
