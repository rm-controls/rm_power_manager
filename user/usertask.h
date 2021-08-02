//
// Created by Lao·Zhu on 2021/2/4.
//

#ifndef _USER_USERTASK_H_
#define _USER_USERTASK_H_

extern unsigned char complex_calibrate_flag;
void UserTask(void *pvParameters);
void LCD_Refresh(void *pvParameters);

#endif //_USER_USERTASK_H_
