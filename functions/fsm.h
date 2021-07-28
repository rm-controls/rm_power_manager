//
// Created by Lao·Zhu on 2021/4/5.
//

#ifndef PROGRAM_FUNCTIONS_FSM_H_
#define PROGRAM_FUNCTIONS_FSM_H_

typedef enum {
    Charge_With_Boost = 1,
    Only_DeliverChassis = 2,
    Chassis_With_Charge = 3,
    Only_Charge = 4,
    All_Off = 5,
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
    Zero_Power_Expect = 6,
} Expect_Power_e;

typedef enum {
    Normal_Optimized = 0,
    ChargeFirst_Optimized = 1,
    UseFirst_Optimized = 2,
    SucapTest_Optimized = 3,
} Optimize_Mode_e;

typedef enum {
    Normal_Mode = 1,
    OverPower_Mode = 2,
    Halt_Mode = 3,
    NoCharge_Mode = 4,
    SucapTest_Mode = 5,
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
