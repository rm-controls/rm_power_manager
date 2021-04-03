//
// Created by Lao·Zhu on 2021/2/4.
//

#ifndef _FUNCTIONS_CALCULATE_H_
#define _FUNCTIONS_CALCULATE_H_

extern float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry,
    V_Chassis, P_Chassis, P_Capacitor, EP_Chassis, W_Capacitor;
extern unsigned short I_CapOffset, I_ChaOffset;
void Calculate_Power(void);
void Sensor_Config(void);

#endif //_FUNCTIONS_CALCULATE_H_
