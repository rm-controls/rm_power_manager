//
// Created by Lao·Zhu on 2021/4/5.
//

#ifndef PROGRAM_FUNCTIONS_PROTECT_H_
#define PROGRAM_FUNCTIONS_PROTECT_H_

extern unsigned char fighting_flag;
extern unsigned char overcurrent_flag;
extern unsigned short referee_avaiflag, referee_time_counter;
void Protect_Task(void *pvParameters);

#endif //PROGRAM_FUNCTIONS_PROTECT_H_
