#include "main.h"
#include "math.h"

extern unsigned char TIM10_Flag;
extern unsigned char TIM11_Flag;

#define COEFFICIENT 3.3f / 4096.0f

int main(void) {
    float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry, V_Chassis, P_Chassis, P_Capacitor, theta = 0;
    unsigned short I_Offset;
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
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 512);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    I_Offset = ADC_FinalResult[0];
    HAL_Delay(100);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    while (1) {
        if (TIM10_Flag == 1) {
            DTP_Package_t pkg = {0};
            float ADC_Resultf[4];
            TIM10_Flag = 0;
            pkg.PID = 0;
            ADC_Resultf[0] = I_Capacitor;
            ADC_Resultf[1] = V_Capacitor;
            ADC_Resultf[2] = PID_Capacitor.User;
            ADC_Resultf[3] = P_Capacitor;
            for (unsigned char counter = 0; counter < 4; counter++) {
                pkg.Data[counter * 2] = FloatToInt16(ADC_Resultf[counter]) >> 8;
                pkg.Data[counter * 2 + 1] = FloatToInt16(ADC_Resultf[counter]) & 0x00ff;
            }
            DTP_Transmit(&pkg);
            PID_Capacitor.User = 20 * (sinf(theta) + 1.0f);
            theta = theta + 0.05f;
        }
        if (TIM11_Flag == 1) {
            TIM11_Flag = 0;
            I_Capacitor = (float) (ADC_FinalResult[0] - I_Offset) * COEFFICIENT * 10.0f;
            I_Chassis = (((float) ADC_FinalResult[1] * COEFFICIENT) - 2.5f) * 10.0f;
            V_Baterry = (float) ADC_FinalResult[2] * COEFFICIENT * 11.0f;
            V_Capacitor = (float) ADC_FinalResult[3] * COEFFICIENT * 11.0f;
            V_Chassis = (float) ADC_FinalResult[4] * COEFFICIENT * 11.0f;

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
            P_CapacitorF.Current_Value = P_Capacitor;
            FirstOrder_Filter_Calculate(&P_CapacitorF);
            P_Capacitor = P_CapacitorF.Current_Result;

            PID_Capacitor.Collect[1] = PID_Capacitor.Collect[0];
            PID_Capacitor.Collect[0] = P_Capacitor;
            PID_Get_Result(&PID_Capacitor);
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, PID_Capacitor.Result / 1);
        }
        if (V_Capacitor >= 15.0f)
            HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    }
}
