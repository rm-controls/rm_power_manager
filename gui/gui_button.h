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

typedef struct {
    unsigned char X_Pos;
    unsigned char Y_Pos;
    unsigned char Width;
    unsigned char Height;
    const char *Text;
} Button_Struct_t;

void GUI_InitButton(Button_Struct_t *button);

#endif //PROGRAM_GUI_GUI_BUTTON_H_
