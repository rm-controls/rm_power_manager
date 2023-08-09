//
// Created by LaoZhu on 2023/5/26.
//

#include "gui_textbox.h"
#include "gui_base.h"
#include "string.h"

void GUI_TextBoxAppend(TextBox_Struct_t *textbox, unsigned short color, char *str) {
    if (textbox == NULL)
        return;
    TextBox_Line_Struct_t *current_line;
    if (textbox->first_line == NULL) {
        textbox->first_line = &textbox->line[0];
        current_line = textbox->first_line;
    } else {
        for (unsigned char counter = 0; counter < 14; ++counter)
            if (textbox->line[counter].text[0] == 0x00 && textbox->line[counter].next_line == NULL)
                current_line = &textbox->line[counter];
    }
    if (textbox->first_line != current_line)
        textbox->last_line->next_line = current_line;
    textbox->last_line = current_line;
    if (strlen(str) > textbox->max_row)
        memcpy(current_line->text, str, textbox->max_row);
    else
        strcpy(current_line->text, str);
    current_line->color = color;
    current_line->next_line = NULL;
    textbox->line_counter++;
    if (textbox->line_counter > textbox->max_line) {
        TextBox_Line_Struct_t *deleting_line;
        deleting_line = textbox->first_line;
        textbox->first_line = deleting_line->next_line;
        deleting_line->color = 0x0000;
        deleting_line->next_line = NULL;
        memset(deleting_line->text, 0x00, 24);
    }
    if (textbox->line_counter > textbox->max_line) {
        gui_draw_rectangle(textbox->X_Pos, textbox->Y_Pos, textbox->Width, textbox->Height, C_WHITE, Filled);
        current_line = textbox->first_line;
        for (unsigned char counter = 0; counter < 14; ++counter) {
            gui_printf(textbox->X_Pos,
                       textbox->Y_Pos + counter * 12,
                       current_line->color,
                       C_WHITE,
                       "%s",
                       current_line->text);
            current_line = current_line->next_line;
            if (current_line->next_line == NULL)
                break;
        }
        textbox->line_counter = textbox->max_line;
    }
    gui_printf(textbox->X_Pos,
               textbox->Y_Pos + (textbox->line_counter - 1) * 12,
               textbox->last_line->color,
               C_WHITE,
               "%s",
               textbox->last_line->text);
}

void GUI_InitTextBox(TextBox_Struct_t *textbox) {
    gui_draw_rectangle(textbox->X_Pos + 1,
                       textbox->Y_Pos + 1,
                       textbox->Width - 2,
                       textbox->Height - 2,
                       C_WHITE,
                       Filled);
    gui_draw_rectangle(textbox->X_Pos, textbox->Y_Pos, textbox->Width, textbox->Height, TEXTBOX_COLOR_EDGE, UnFilled);
    textbox->X_Pos = textbox->X_Pos + 1;
    textbox->Y_Pos = textbox->Y_Pos + 1;
    textbox->Width = textbox->Width - 2;
    textbox->Height = textbox->Height - 2;
    for (unsigned char counter = 0; counter < 13; ++counter) {
        textbox->line[counter].color = 0x0000;
        textbox->line[counter].next_line = NULL;
        memset(textbox->line[counter].text, 0x00, 24);
    }
    textbox->first_line = NULL;
    textbox->last_line = NULL;
    textbox->max_line = textbox->Height / 12;
    textbox->max_row = textbox->Width / 6;
    textbox->line_counter = 0;
}
