//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef PROGRAM_FORMS_INFO_FORM_H_
#define PROGRAM_FORMS_INFO_FORM_H_

typedef enum {
  Switch_To_OverPower = 0,
  Switch_To_Normal = 1,
  Switch_To_Transition = 2,
  Switch_To_Halt = 3,
} Log_Event_e;

void LogForm_Init(void);
void LogForm_Update(void);

#endif //PROGRAM_FORMS_INFO_FORM_H_
