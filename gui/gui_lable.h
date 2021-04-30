//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef PROGRAM_GUI_GUI_LABLE_H_
#define PROGRAM_GUI_GUI_LABLE_H_

typedef struct {
    unsigned char X_Pos;
    unsigned char Y_Pos;
    unsigned short Color;
    const char *Text;
} Lable_Struct_t;

void GUI_InitLable(Lable_Struct_t *lable);
void GUI_LableSetText(Lable_Struct_t *lable, const char *fmt, ...);

#endif //PROGRAM_GUI_GUI_LABLE_H_
