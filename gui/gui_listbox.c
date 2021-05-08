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
    ListBox->Widget_Type = ListBox_Widget_Type;
    if (ListBox->FirstItem != NULL) {
        ListBox_Item_Struct_t *CurrentItem = ListBox->FirstItem, *TmpItem;
        while (1) {
            if (CurrentItem->Next_ListBox_Item != NULL) {
                TmpItem = CurrentItem->Next_ListBox_Item;
                vPortFree(CurrentItem);
                CurrentItem = TmpItem;
            } else {
                vPortFree(CurrentItem);
                break;
            }
        }
    }
    ListBox->FirstItem = NULL;
    GUI_DrawRectangle(ListBox->X_Pos, ListBox->Y_Pos, ListBox->Width, ListBox->Height, C_WHITE, Filled);
    GUI_DrawRectangle(ListBox->X_Pos, ListBox->Y_Pos, ListBox->Width, ListBox->Height, LISTBOX_COLOR_EDGE, UnFilled);
}

void GUI_ListBoxDisplayItem(ListBox_Struct_t *ListBox,
                            unsigned char *str,
                            unsigned char index,
                            unsigned char InvertColor) {
    unsigned char n = 0, row = ListBox->X_Pos + 2;
    unsigned char column = ListBox->Y_Pos + index * 12 + 2;
    if (InvertColor == 0) {
        while (str[n] != '\0') {
            LCD_SendChar(row, column, str[n], C_BLACK, C_WHITE);
            row += 6;
            n++;
        }
        GUI_DrawRectangle(row, column, (20 - n) * 6 - 4, 12, C_WHITE, Filled);
    } else {
        while (str[n] != '\0') {
            LCD_SendChar(row, column, str[n], C_BLACK, TransColor(LISTBOX_COLOR_FOCUS));
            row += 6;
            n++;
        }
        GUI_DrawRectangle(row, column, (20 - n) * 6 - 4, 12, LISTBOX_COLOR_FOCUS, Filled);
    }
}

ListBox_Item_Struct_t *GUI_ListBoxFindItem(ListBox_Struct_t *ListBox, unsigned char ItemIndex) {
    ListBox_Item_Struct_t *Current_ListBoxItem_Tmp = ListBox->FirstItem;
    for (unsigned char counter = 0; counter < ItemIndex; counter++)
        Current_ListBoxItem_Tmp = Current_ListBoxItem_Tmp->Next_ListBox_Item;
    return Current_ListBoxItem_Tmp;
}

void GUI_ListBoxUpdateContents(ListBox_Struct_t *ListBox) {
    unsigned char FirstItem_Index = ListBox->ItemIndex - ListBox->DisplayIndex;
    ListBox_Item_Struct_t *Tmp_Item = GUI_ListBoxFindItem(ListBox, FirstItem_Index);
    while (1) {
        if ((FirstItem_Index - ListBox->ItemIndex + ListBox->DisplayIndex) == ListBox->DisplayIndex)
            GUI_ListBoxDisplayItem(ListBox,
                                   Tmp_Item->ItemText,
                                   FirstItem_Index - ListBox->ItemIndex + ListBox->DisplayIndex,
                                   1);
        else
            GUI_ListBoxDisplayItem(ListBox,
                                   Tmp_Item->ItemText,
                                   FirstItem_Index - ListBox->ItemIndex + ListBox->DisplayIndex,
                                   0);
        if (FirstItem_Index == (ListBox->ItemNumber - 1)
            || (FirstItem_Index - ListBox->ItemIndex + ListBox->DisplayIndex)
                == ((ListBox->Height - 4) / 12) - 1)
            break;
        FirstItem_Index++;
        Tmp_Item = GUI_ListBoxFindItem(ListBox, FirstItem_Index);
    }
}

void GUI_ListBoxUpdate(ListBox_Struct_t *ListBox, unsigned char keynum) {
    ListBox_Item_Struct_t *Current_Item = ListBox->FirstItem;
    ListBox->Status = ListBox_Focus;
    switch (keynum) {
        case 0:
            if (ListBox->FirstItem->Next_ListBox_Item != NULL) {
                unsigned char item_counter_tmp = 0;
                while (Current_Item->Next_ListBox_Item != NULL) {
                    if (item_counter_tmp < (ListBox->Height - 4) / 12)
                        GUI_ListBoxDisplayItem(ListBox, Current_Item->ItemText, item_counter_tmp, 0);
                    else
                        break;
                    Current_Item = Current_Item->Next_ListBox_Item;
                    item_counter_tmp++;
                }
            } else if (ListBox->FirstItem != NULL)
                GUI_ListBoxDisplayItem(ListBox, ListBox->FirstItem->ItemText, 0, 0);
            ListBox->Status = ListBox_Normal;
            break;
        case Center_Key:ListBox->CallbackFunction(ListBox, Center_Key);
            break;
        case Up_Key:
            if (ListBox->ItemIndex == 0) {
                ListBox->ItemIndex = ListBox->ItemNumber - 1;
                ListBox->DisplayIndex = ((ListBox->Height - 4) / 12) - 1;
                ListBox->DisplayIndex =
                    ListBox->ItemIndex < ListBox->DisplayIndex ? ListBox->ItemIndex : ListBox->DisplayIndex;
            } else {
                ListBox->ItemIndex--;
                if (ListBox->DisplayIndex != 0)
                    ListBox->DisplayIndex--;
            }
            GUI_ListBoxUpdateContents(ListBox);
            break;
        case Down_Key:
            if (ListBox->ItemIndex == (ListBox->ItemNumber - 1)) {
                ListBox->ItemIndex = 0;
                ListBox->DisplayIndex = 0;
            } else {
                ListBox->ItemIndex++;
                if (ListBox->DisplayIndex != (((ListBox->Height - 4) / 12) - 1))
                    ListBox->DisplayIndex++;
            }
            GUI_ListBoxUpdateContents(ListBox);
            break;
        default:break;
    }
}

void GUI_ListBoxAddItem(ListBox_Struct_t *ListBox, const char *fmt, ...) {
    ListBox_Item_Struct_t *NewItem = pvPortMalloc(sizeof(ListBox_Item_Struct_t));
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
    if (ListBox->ItemNumber < (ListBox->Height - 4) / 12)
        GUI_ListBoxDisplayItem(ListBox, NewItem->ItemText, ListBox->ItemNumber, 0);
    ListBox->ItemNumber++;
}
