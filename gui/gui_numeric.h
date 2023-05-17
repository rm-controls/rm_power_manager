//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef PROGRAM_GUI_GUI_NUMERIC_H_
#define PROGRAM_GUI_GUI_NUMERIC_H_

#define NUMERIC_NORMAL_EDGE 0xad75

typedef enum {
    Numeric_Button_Normal = 0,
    Numeric_Up_Button_Focus = 1,
    Numeric_Up_Button_Click = 2,
    Numeric_Down_Button_Focus = 4,
    Numeric_Down_Button_Click = 5,
} Numeric_Button_State_e;

typedef struct NUMERIC_TYPE_T {
    unsigned char X_Pos;
    unsigned char Y_Pos;
    unsigned char Width;
    unsigned char Height;
    const char *Text;
    void *NextNumeric;
    void (*CallbackFunction)(void *Object, unsigned char Key);
    unsigned char Widget_Type;
    Numeric_Button_State_e Status;
    unsigned int Value;
} Numeric_Struct_t;

void GUI_InitNumeric(Numeric_Struct_t *Numeric);
void GUI_UpdateNumeric(Numeric_Struct_t *Numeric, Numeric_Button_State_e btn_state);
void GUI_NumericSetText(Numeric_Struct_t *Numeric, const char *fmt, ...);

#endif //PROGRAM_GUI_GUI_NUMERIC_H_
