//
// Created by Lao·Zhu on 2021/2/4.
//

#ifndef _FUNCTIONS_CALCULATE_H_
#define _FUNCTIONS_CALCULATE_H_

typedef struct {
    float Rw;
    float Pr[2];
    float Pd[2];
    float Iw[2];
} Power_Calibrate_t;

extern Power_Calibrate_t Capacitor_Calibratel, Capacitor_Calibrateh;
extern float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry,
    V_Chassis, P_Chassis, P_Capacitor, EP_Chassis, W_Capacitor, Capacitor_Percent;
extern unsigned short I_CapOffset, I_ChaOffset;
void Calculate_Power(void);
void Sensor_Config(void);
void Calibrate_Powerl(void);
void Calibrate_Powerh(void);
void Referee_Power_Callback(void);

#endif //_FUNCTIONS_CALCULATE_H_
