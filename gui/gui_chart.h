//
// Created by Lao·Zhu on 2021/4/30.
//

#ifndef PROGRAM_GUI_GUI_CHART_H_
#define PROGRAM_GUI_GUI_CHART_H_

typedef enum {
    No_Coordinate_Display = 0,
    Rectangular_Coordinate = 1,
} Coordinate_Style_e;
typedef enum {
    No_Value_Display = 0,
    Display_8x16_Value = 1,
} Value_Style_e;

typedef struct {
    unsigned char X_Pos;
    unsigned char Y_Pos;
    unsigned char Width;
    unsigned char Height;
    unsigned char Value_Style;
    unsigned char Coordinate_Style;
    float MaxAmplification;
} Chart_Struct_t;

typedef struct {
    Chart_Struct_t *Chart;
    float Amplification_Factor;
    unsigned short Color;
    unsigned char Pos_Counter;
    unsigned char Y_Pos[127];
} Curve_Struct_t;

void GUI_InitChart(Chart_Struct_t *Chart);
void GUI_InitCurve(Curve_Struct_t *Curve, Chart_Struct_t *Chart, unsigned short color);
void GUI_CurveAppend(Curve_Struct_t *Curve, float Value);

#endif //PROGRAM_GUI_GUI_CHART_H_
