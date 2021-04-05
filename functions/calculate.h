//
// Created by Lao·Zhu on 2021/2/4.
//

#ifndef _FUNCTIONS_CALCULATE_H_
#define _FUNCTIONS_CALCULATE_H_

typedef struct {
    float a;
    float b;
    float x1;
    float x2;
    float y1;
    float y2;
} Power_Calibrate_t;

extern Power_Calibrate_t Capacitor_Calibrate, Chassis_Calibrate;
extern float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry,
    V_Chassis, P_Chassis, P_Capacitor, EP_Chassis, W_Capacitor;
extern unsigned short I_CapOffset, I_ChaOffset;
void Calculate_Power(void);
void Sensor_Config(void);
void Calibrate_Power(void);

#endif //_FUNCTIONS_CALCULATE_H_
