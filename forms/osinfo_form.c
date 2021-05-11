//
// Created by Lao·Zhu on 2021/5/12.
//

#include "main.h"

Button_Struct_t TurnBack_Button3;
unsigned char osinfo_update_counter = 0;

unsigned char OSInfo_Printf(unsigned char row, unsigned char column, char *fmt) {
    unsigned char n = 0;
    while (fmt[n] != '\n') {
        LCD_SendChar(row, column, fmt[n], C_BLACK, C_WHITE);
        row += 6;
        n++;
    }
    return n;
}

void RefreshTaskInfo(void) {
    char *buf_source = pvPortMalloc(512), *buf = buf_source;
    vTaskList(buf);
    OSInfo_Printf(2, 3, "Name State Prio Stack\n");
    buf += OSInfo_Printf(14, 18, buf) + 1;
    buf += OSInfo_Printf(14, 32, buf) + 1;
    buf += OSInfo_Printf(14, 46, buf) + 1;
    buf += OSInfo_Printf(14, 60, buf) + 1;
    buf += OSInfo_Printf(14, 74, buf) + 1;
    buf += OSInfo_Printf(14, 88, buf) + 1;
    buf += OSInfo_Printf(14, 102, buf) + 1;
    OSInfo_Printf(14, 116, buf);
    vPortFree(buf_source);
}

void TurnBack_Button3_Callback(void *object, unsigned char key) {
    Form_Info_Structure.Form_Index = Log_Form_Index;
    LogForm_Init();
}

void OSInfoForm_Update(void) {
    osinfo_update_counter++;
    if (osinfo_update_counter == 20) {
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

    GUI_Clear(C_WHITE);
    GUI_InitButton(&TurnBack_Button3);
    RefreshTaskInfo();
}
