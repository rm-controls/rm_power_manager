#ifndef _LCD_H_
#define _LCD_H_

void LCD_Config();
void LCD_Scan(unsigned char mode);
void Lcd_Write_Data(unsigned int dat16);
void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

#endif
