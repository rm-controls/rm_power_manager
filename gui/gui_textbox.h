//
// Created by LaoZhu on 2023/5/26.
//

#ifndef POWER_MANAGER_GUI_GUI_TEXTBOX_H_
#define POWER_MANAGER_GUI_GUI_TEXTBOX_H_

#define TEXTBOX_COLOR_EDGE      gui_trans_color(0xad56)

typedef struct TEXTBOX_LINE_STRUCT {
  char text[22];
  unsigned short color;
  struct TEXTBOX_LINE_STRUCT *next_line;
} TextBox_Line_Struct_t;

typedef struct {
  unsigned char X_Pos;
  unsigned char Y_Pos;
  unsigned char Width;
  unsigned char Height;
  TextBox_Line_Struct_t line[14];
  TextBox_Line_Struct_t *first_line;
  TextBox_Line_Struct_t *last_line;
  unsigned char max_line;
  unsigned char max_row;
  unsigned char line_counter;
} TextBox_Struct_t;

void GUI_TextBoxAppend(TextBox_Struct_t *textbox, unsigned short color, char *str);
void GUI_InitTextBox(TextBox_Struct_t *textbox);

#endif //POWER_MANAGER_GUI_GUI_TEXTBOX_H_
