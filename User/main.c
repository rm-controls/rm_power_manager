#include "main.h"

extern unsigned short INTFlag;

#define COEFFICIENT 3.3f / 4096.0f
float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry, V_Chassis, P_Chassis, P_Capacitor;

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Config();
    DMA_Config();
    ADC_Config();
    CAN_Config();
    DAC_Config();
    UART_Config();
    TIM_Config();
    FirstOrder_Filter_Config();
    PID_Value_Config();
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 128);
    while (1) {
        if ((INTFlag & (~(1 << TIM6_INT_BIT))) == 1) {
            DTP_Package_t pkg = {0};
            float ADC_Resultf[4];
            pkg.PID = 0;
            ADC_Resultf[0] = I_Capacitor;
            ADC_Resultf[1] = V_Capacitor;
            ADC_Resultf[2] = V_Baterry;
            ADC_Resultf[3] = P_Capacitor;
            for (unsigned char counter = 0; counter < 4; counter++) {
                pkg.Data[counter * 2] = FloatToInt16(ADC_Resultf[counter]) >> 8;
                pkg.Data[counter * 2 + 1] = FloatToInt16(ADC_Resultf[counter]) & 0x00ff;
            }
            DTP_Transmit(&pkg);
        } else if ((INTFlag & (~(1 << TIM7_INT_BIT))) == 1) {

            I_Capacitor = (((float) ADC_Result[0] * COEFFICIENT) - 2.5f) * 8.0f;
            I_Chassis = (((float) ADC_Result[1] * COEFFICIENT) - 2.5f) * 8.0f;
            V_Baterry = (float) ADC_Result[2] * COEFFICIENT * 11.0f;
            V_Capacitor = (float) ADC_Result[3] * COEFFICIENT * 11.0f;
            V_Chassis = (float) ADC_Result[4] * COEFFICIENT * 11.0f;

            I_CapacitorF.Current_Value = I_Capacitor;
            FirstOrder_Filter_Calculate(&I_CapacitorF);
            I_Capacitor = I_CapacitorF.Current_Result;

            I_ChassisF.Current_Value = I_Chassis;
            FirstOrder_Filter_Calculate(&I_ChassisF);
            I_Chassis = I_ChassisF.Current_Result;

            V_BaterryF.Current_Value = V_Baterry;
            FirstOrder_Filter_Calculate(&V_BaterryF);
            V_Baterry = V_BaterryF.Current_Result;

            V_CapacitorF.Current_Value = V_Capacitor;
            FirstOrder_Filter_Calculate(&V_CapacitorF);
            V_Capacitor = V_CapacitorF.Current_Result;

            V_ChassisF.Current_Value = V_Chassis;
            FirstOrder_Filter_Calculate(&V_ChassisF);
            V_Chassis = V_ChassisF.Current_Result;

            P_Chassis = I_Chassis * V_Chassis;
            P_Capacitor = V_Baterry * I_Capacitor;

        } else if ((INTFlag & (~(1 << TIM10_INT_BIT))) == 1) {
            //        PID_Capacitor.Collect[1] = PID_Capacitor.Collect[0];
//        PID_Capacitor.Collect[0] = P_Capacitor;
//        PID_Get_Result(&PID_Capacitor);
        }
        if (V_Capacitor >= 15.0f)
            HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    }
}
