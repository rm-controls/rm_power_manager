//
// Created by Lao·Zhu on 2021/5/12.
//

#include "main.h"

static Button_Struct_t TurnBack_Button3;
static unsigned char osinfo_update_counter = 0;

static unsigned char OSInfo_Printf(unsigned char row, unsigned char column, char *fmt) {
    unsigned char n = 0;
    while (fmt[n] != '\n') {
        if (fmt[n] == '\t')
            fmt[n] = ' ';
        if (fmt[n] >= 32 && fmt[n] <= 127) {
            gui_putchar(row, column, fmt[n], 0x0000, 0xFFFF);
            row += 6;
        }
        n++;
    }
    return n;
}

static void RefreshTaskInfo(void) {
    char *buf_source = pvPortMalloc(1024), *buf = buf_source;
    vTaskList(buf);
    OSInfo_Printf(2, 6, "Name State Prio Stack\n");
    buf += OSInfo_Printf(2, 22, buf) + 1;
    buf += OSInfo_Printf(2, 36, buf) + 1;
    buf += OSInfo_Printf(2, 50, buf) + 1;
    buf += OSInfo_Printf(2, 64, buf) + 1;
    buf += OSInfo_Printf(2, 78, buf) + 1;
    OSInfo_Printf(2, 92, buf);
    vPortFree(buf_source);
}

static void TurnBack_Button3_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Log_Form_Index;
    LogForm_Init();
}

void OSInfoForm_Update(void) {
    osinfo_update_counter++;
    if (osinfo_update_counter == 10) {
        RefreshTaskInfo();
        osinfo_update_counter = 0;
    }
}

void OSInfoForm_Init(void) {
    Form_Info_Structure.Widget_Index = 0;
    FirstWidget = (GUI_Object_Struct_t *) &TurnBack_Button3;

    TurnBack_Button3.X_Pos = 24;
    TurnBack_Button3.Y_Pos = 136;
    TurnBack_Button3.Width = 80;
    TurnBack_Button3.Height = 20;
    TurnBack_Button3.Text = "TurnBack";
    TurnBack_Button3.NextButton = NULL;
    TurnBack_Button3.CallbackFunction = TurnBack_Button3_Callback;

    gui_clear_screen(C_WHITE);
    GUI_InitButton(&TurnBack_Button3);
    RefreshTaskInfo();
}
