//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef PROGRAM_FORMS_FORM_LOGIC_H_
#define PROGRAM_FORMS_FORM_LOGIC_H_

typedef enum {
    Main_Form_Index = 0,
    Log_Form_Index = 1,
    Settings_Form_Index = 2,
} Form_Index_e;

typedef struct {
    unsigned char Form_Index;
    unsigned char Widget_Index;
} Form_Info_t;

extern Form_Info_t Form_Info_Structure;
void Form_UpdateEvent(void);

#endif //PROGRAM_FORMS_FORM_LOGIC_H_
