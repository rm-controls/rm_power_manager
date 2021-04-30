//
// Created by Lao·Zhu on 2021/4/29.
//

#ifndef PROGRAM_GUI_GUI_BUTTON_H_
#define PROGRAM_GUI_GUI_BUTTON_H_

#define BUTTON_COLOR_NORMAL 0xe73c
#define BUTTON_COLOR_NORMAL_EDGE 0xce59

#define BUTTON_COLOR_FOCUS 0xe79f
#define BUTTON_COLOR_FOCUS_EDGE 0x03da

#define BUTTON_COLOR_CLICK 0xcf1e
#define BUTTON_COLOR_CLICK_EDGE 0x02b3

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
    Button_State_e Status;
    const char *Text;
    struct BUTTON_TYPE_T *NextButton;
    void (*CallbackFunction)(struct BUTTON_TYPE_T *Button);
} Button_Struct_t;

void GUI_InitButton(Button_Struct_t *button);
void GUI_UpdateButton(Button_Struct_t *button, Button_State_e status);

#endif //PROGRAM_GUI_GUI_BUTTON_H_
