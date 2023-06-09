//
// Created by Lao·Zhu on 2021/4/29.
//

#ifndef POWER_MANAGER_GUI_GUI_BUTTON_H_
#define POWER_MANAGER_GUI_GUI_BUTTON_H_

#define BUTTON_COLOR_NORMAL         gui_trans_color(0xe73c)
#define BUTTON_COLOR_NORMAL_EDGE    gui_trans_color(0xce59)

#define BUTTON_COLOR_FOCUS          gui_trans_color(0xe79f)
#define BUTTON_COLOR_FOCUS_EDGE     gui_trans_color(0x03da)

#define BUTTON_COLOR_CLICK          gui_trans_color(0xcf1e)
#define BUTTON_COLOR_CLICK_EDGE     gui_trans_color(0x02b3)

typedef enum {
  Button_Normal = 0,
  Button_Focus = 1,
  Button_Click = 2,
} Button_State_e;

typedef struct BUTTON_TYPE_T {
  unsigned char X_Pos;
  unsigned char Y_Pos;
  unsigned char Width;
  unsigned char Height;
  const char *Text;
  void *NextButton;
  void (*CallbackFunction)(void *Object, unsigned char Key);
  unsigned char Widget_Type;
  Button_State_e Status;
} Button_Struct_t;

void GUI_InitButton(Button_Struct_t *button);
void GUI_UpdateButton(Button_Struct_t *button, Button_State_e status);

#endif  // POWER_MANAGER_GUI_GUI_BUTTON_H_
