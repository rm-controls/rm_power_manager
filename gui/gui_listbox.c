//
// Created by Lao·Zhu on 2021/5/6.
//

#include "gui_listbox.h"
#include "gui_base.h"
#include "key.h"
#include "FreeRTOS.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

void GUI_InitListBox(ListBox_Struct_t *ListBox) {
    ListBox->ItemIndex = 0;
    ListBox->DisplayIndex = 0;
    ListBox->FirstItem = NULL;
    GUI_DrawRectangle(ListBox->X_Pos, ListBox->Y_Pos, ListBox->Width, ListBox->Height, C_WHITE, Filled);
    GUI_DrawRectangle(ListBox->X_Pos, ListBox->Y_Pos, ListBox->Width, ListBox->Height, LISTBOX_COLOR_EDGE, UnFilled);
}

void GUI_ListBoxUpdate(ListBox_Struct_t *ListBox, unsigned char keynum) {
    ListBox_Item_Struct_t *Current_Item = ListBox->FirstItem;
    switch (keynum) {
        case 0:
            if (ListBox->FirstItem->Next_ListBox_Item != NULL) {
                while (Current_Item->Next_ListBox_Item != NULL) {
                    unsigned char n = 0, row = ListBox->X_Pos + 2;
                    unsigned char column = ListBox->Y_Pos + Current_Item->ItemNumber * 12 + 2;
                    if ((Current_Item->ItemNumber + 1) < (ListBox->Height - 4) / 12)
                        while (Current_Item->ItemText[n] != '\0') {
                            LCD_SendChar(row, column, Current_Item->ItemText[n], C_BLACK, C_WHITE);
                            row += 6;
                            n++;
                        }
                    else
                        break;
                    Current_Item = Current_Item->Next_ListBox_Item;
                }
            } else {
                unsigned char n = 0, row = ListBox->X_Pos + 2;
                unsigned char column = ListBox->Y_Pos + ListBox->FirstItem->ItemNumber * 12 + 2;
                while (ListBox->FirstItem->ItemText[n] != '\0') {
                    LCD_SendChar(row, column, ListBox->FirstItem->ItemText[n], C_BLACK, C_WHITE);
                    row += 6;
                    n++;
                }
            }
            break;
        case Up_Key:break;
        case Down_Key:break;
        default:break;
    }
}

void GUI_ListBoxAddItem(ListBox_Struct_t *ListBox, const char *fmt, ...) {
    ListBox_Item_Struct_t *NewItem = pvPortMalloc(sizeof(ListBox_Item_Struct_t));
    unsigned char n = 0, row = ListBox->X_Pos + 2, column = 0;
    va_list ap;
    memset(NewItem->ItemText, '\0', 21);
    NewItem->Next_ListBox_Item = NULL;
    va_start(ap, fmt);
    vsprintf((char *) NewItem->ItemText, fmt, ap);
    va_end(ap);
    if (ListBox->FirstItem == NULL) {
        ListBox->FirstItem = NewItem;
        ListBox->FirstItem->ItemNumber = 0;
        ListBox->FirstItem->Previous_ListBox_Item = ListBox->FirstItem;
    } else {
        ListBox_Item_Struct_t *Current_Item = ListBox->FirstItem;
        while (Current_Item->Next_ListBox_Item != NULL)
            Current_Item = Current_Item->Next_ListBox_Item;
        Current_Item->Next_ListBox_Item = NewItem;
        Current_Item->Next_ListBox_Item->Previous_ListBox_Item = Current_Item->Next_ListBox_Item;
        Current_Item->Next_ListBox_Item->ItemNumber = Current_Item->ItemNumber + 1;
        ListBox->FirstItem->Previous_ListBox_Item = Current_Item->Next_ListBox_Item;
    }
    column = ListBox->Y_Pos + NewItem->ItemNumber * 12 + 2;
    if ((NewItem->ItemNumber + 1) < (ListBox->Height - 4) / 12)
        while (NewItem->ItemText[n] != '\0') {
            LCD_SendChar(row, column, NewItem->ItemText[n], C_BLACK, C_WHITE);
            row += 6;
            n++;
        }
}
