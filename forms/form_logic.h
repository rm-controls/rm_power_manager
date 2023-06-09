//
// Created by Lao·Zhu on 2021/5/1.
//

#ifndef POWER_MANAGER_FORMS_FORM_LOGIC_H_
#define POWER_MANAGER_FORMS_FORM_LOGIC_H_

typedef enum {
  Main_Form_Index = 0,
  Log_Form_Index = 1,
  Settings_Form_Index = 2,
  OSInfo_Form_Index = 3,
  SysInfo_Form_Index = 4,
  LogView_Form_Index = 5,
  SelfCheck_Form_Index = 6,
} Form_Index_e;

typedef struct {
  unsigned char Form_Index;
  unsigned char Widget_Index;
} Form_Info_t;

typedef struct GUI_OBJECT_TYPE_T {
  unsigned char X_Pos;
  unsigned char Y_Pos;
  unsigned char Width;
  unsigned char Height;
  const char *Text;
  void *NextObject;

  void (*CallbackFunction)(void *Object, unsigned char Key);

  unsigned char Widget_Type;
} GUI_Object_Struct_t;

typedef enum {
  Button_Widget_Type = 1,
  Numeric_Widget_Type = 2,
  ListBox_Widget_Type = 3,
} Widget_Type_e;

extern GUI_Object_Struct_t *FirstWidget;
extern Form_Info_t Form_Info_Structure;
extern unsigned char lcd_off_flag;

void Form_UpdateEvent(void);

#endif  // POWER_MANAGER_FORMS_FORM_LOGIC_H_
