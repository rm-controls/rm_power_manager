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
#include "form_logic.h"

void GUI_InitListBox(ListBox_Struct_t *ListBox) {
    ListBox->ItemIndex = 0;
    ListBox->DisplayIndex = 0;
    ListBox->ItemNumber = 0;
    ListBox->FirstItem = NULL;
    ListBox->Widget_Type = ListBox_Widget_Type;
    GUI_DrawRectangle(ListBox->X_Pos, ListBox->Y_Pos, ListBox->Width, ListBox->Height, C_WHITE, Filled);
    GUI_DrawRectangle(ListBox->X_Pos, ListBox->Y_Pos, ListBox->Width, ListBox->Height, LISTBOX_COLOR_EDGE, UnFilled);
}

ListBox_Item_Struct_t *GUI_ListBoxFindItem(ListBox_Struct_t *ListBox, unsigned char ItemIndex) {
    ListBox_Item_Struct_t *Current_ListBoxItem_Tmp = ListBox->FirstItem;
    for (unsigned char counter = 0; counter < ItemIndex; counter++)
        Current_ListBoxItem_Tmp = Current_ListBoxItem_Tmp->Next_ListBox_Item;
    return Current_ListBoxItem_Tmp;
}

void GUI_ListBoxUpdate(ListBox_Struct_t *ListBox, unsigned char keynum) {
    ListBox_Item_Struct_t *Current_Item = ListBox->FirstItem;
    switch (keynum) {
        case 0:
            if (ListBox->FirstItem->Next_ListBox_Item != NULL) {
                unsigned char item_counter_tmp = 0;
                while (Current_Item->Next_ListBox_Item != NULL) {
                    unsigned char n = 0, row = ListBox->X_Pos + 2;
                    unsigned char column = ListBox->Y_Pos + item_counter_tmp * 12 + 2;
                    if ((item_counter_tmp + 1) < (ListBox->Height - 4) / 12)
                        while (Current_Item->ItemText[n] != '\0') {
                            LCD_SendChar(row, column, Current_Item->ItemText[n], C_BLACK, C_WHITE);
                            row += 6;
                            n++;
                        }
                    else
                        break;
                    Current_Item = Current_Item->Next_ListBox_Item;
                    item_counter_tmp++;
                }
            } else {
                unsigned char n = 0, row = ListBox->X_Pos + 2;
                unsigned char column = ListBox->Y_Pos + 2;
                while (ListBox->FirstItem->ItemText[n] != '\0') {
                    LCD_SendChar(row, column, ListBox->FirstItem->ItemText[n], C_BLACK, C_WHITE);
                    row += 6;
                    n++;
                }
            }
            break;
        case Center_Key:ListBox->CallbackFunction(ListBox, Center_Key);
            break;
        case Up_Key:
            if (ListBox->ItemIndex == 0) {
                ListBox->ItemIndex = ListBox->ItemNumber;
                ListBox->DisplayIndex = ((ListBox->Height - 4) / 12) - 1;
                ListBox->DisplayIndex =
                    ListBox->ItemIndex < ListBox->DisplayIndex ? ListBox->ItemIndex : ListBox->DisplayIndex;
            } else {
                ListBox->ItemIndex--;
                if (ListBox->DisplayIndex != 0)
                    ListBox->DisplayIndex--;
            }

            break;
        case Down_Key:
            if (ListBox->ItemIndex == ListBox->ItemNumber)
                ListBox->ItemIndex = 0;
            else
                ListBox->ItemIndex++;
            break;
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
    } else {
        ListBox_Item_Struct_t *Current_Item = ListBox->FirstItem;
        while (Current_Item->Next_ListBox_Item != NULL)
            Current_Item = Current_Item->Next_ListBox_Item;
        Current_Item->Next_ListBox_Item = NewItem;
    }
    column = ListBox->Y_Pos + ListBox->ItemNumber * 12 + 2;
    if ((ListBox->ItemNumber + 1) < (ListBox->Height - 4) / 12)
        while (NewItem->ItemText[n] != '\0') {
            LCD_SendChar(row, column, NewItem->ItemText[n], C_BLACK, C_WHITE);
            row += 6;
            n++;
        }
    ListBox->ItemNumber++;
}
