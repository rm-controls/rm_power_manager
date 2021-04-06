//
// Created by Lao·Zhu on 2021/4/6.
//

#ifndef PROGRAM_FUNCTIONS_DATASAVE_H_
#define PROGRAM_FUNCTIONS_DATASAVE_H_

typedef enum {
    RePowerOn_Reset = 1,
    Watchdog_Reset = 2,
    Hardfault_Reset = 3,
    UsageFault_Reset = 4,
    BusFault_Reset = 5,
    NMI_Reset = 6,
} Saving_Reason_e;

void DataSave_To_Flash(Saving_Reason_e reason);

#endif //PROGRAM_FUNCTIONS_DATASAVE_H_
