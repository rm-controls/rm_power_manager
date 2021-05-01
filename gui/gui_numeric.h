//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef PROGRAM_GUI_GUI_NUMERIC_H_
#define PROGRAM_GUI_GUI_NUMERIC_H_

#include "key.h"

typedef struct NUMERIC_TYPE_T {
    unsigned char X_Pos;
    unsigned char Y_Pos;
    unsigned char Width;
    unsigned char Height;
    const char *Text;
    void *NextNumeric;
    void (*CallbackFunction)(void *Object, Key_Num_e Key);
    unsigned char Widget_Type;
    float Value;
} Numeric_Struct_t;

#endif //PROGRAM_GUI_GUI_NUMERIC_H_
