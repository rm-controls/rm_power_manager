//
// Created by Lao·Zhu on 2021/4/5.
//

#ifndef PROGRAM_FUNCTIONS_FSM_H_
#define PROGRAM_FUNCTIONS_FSM_H_

typedef enum {
    Only_Charge = 1,
    Only_PMOS = 2,
    PMOS_With_Charge = 3,
    All_Off = 4,
} Topology_Mode_e;

typedef enum {
    Zero_Power_Charge = 1,
    Const_Power_Charge = 2,
    Proportional_Charge = 3,
    Full_Power_Charge = 4,
    Remain_Power_Charge = 5,
} Charge_Mode_e;

typedef enum {
    Const_Power_Expect = 1,
    Proportional_Expect = 2,
    Full_Power_Expect = 3,
    Remain_Power_Expect = 4,
    OverPower_Expect = 5,
} Expect_Power_e;

typedef enum {
    Normal_Mode = 1,
    OverPower_Mode = 2,
    Halt_Mode = 3,
    Transition_Mode = 4,
} FSM_Mode_e;

typedef struct {
    Topology_Mode_e Typology_Mode;
    Charge_Mode_e Charge_Mode;
    Expect_Power_e Expect_Mode;
    Charge_Mode_e uCharge_Mode;
    Expect_Power_e uExpect_Mode;
    FSM_Mode_e FSM_Mode;
    unsigned char Expect_Power;
    unsigned char Charge_Power;
    unsigned char Max_Power;
    float P_Expect;
    float P_Charge;
} FSM_Status_t;

extern FSM_Status_t FSM_Status;
void FSM_Task(void *pvParameters);

#endif //PROGRAM_FUNCTIONS_FSM_H_
