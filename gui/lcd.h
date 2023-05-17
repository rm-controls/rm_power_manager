//
// Created by LaoZhu on 2023/5/17.
//

#ifndef POWER_MANAGER_GUI_LCD_H_
#define POWER_MANAGER_GUI_LCD_H_

void lcd_config(void);
void lcd_scan_mode(unsigned char mode);
void lcd_address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void lcd_refresh_once(volatile unsigned char *buffer);

#endif //POWER_MANAGER_GUI_LCD_H_
