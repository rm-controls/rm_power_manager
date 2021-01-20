#include "main.h"
#include "math.h"

#define COEFFICIENT 3.3f / 4096.0f

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Config();
    DMA_Config();
    ADC_Config();
    CAN_Config();
    DAC_Config();
    UART_Config();
    PID_Capacitor.Kp = 100;
    PID_Capacitor.Ki = 0;
    PID_Capacitor.Kd = 0;
    PID_Capacitor.User = 20;
    PID_Capacitor.I_Sum_Max = 100;
    PID_Capacitor.Maxinum = 4095;
    PID_Capacitor.Minium = 1;
    PID_Capacitor.Offset = 0;
    while (1) {
        float I_Capacitor, I_Chassis, V_Capacitor, V_Baterry, V_Chassis, P_Chassis, P_Capacitor;
        DTP_Package_t pkg = {0};
        I_Capacitor = (((float) ADC_Result[0] * COEFFICIENT) - 2.5f) * 8.0f;
        I_Chassis = (((float) ADC_Result[1] * COEFFICIENT) - 2.5f) * 8.0f;
        V_Baterry = (float) ADC_Result[2] * COEFFICIENT * 11.0f;
        V_Capacitor = (float) ADC_Result[3] * COEFFICIENT * 11.0f;
        V_Chassis = (float) ADC_Result[4] * COEFFICIENT * 11.0f;
        P_Chassis = I_Chassis * V_Chassis;
        P_Capacitor = V_Baterry * I_Capacitor;
//
        if (V_Capacitor >= 15.0f)
            HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, 1);
        else
            HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);

//        PID_Capacitor.Collect[1] = PID_Capacitor.Collect[0];
//        PID_Capacitor.Collect[0] = P_Capacitor;
//        PID_Get_Result(&PID_Capacitor);
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2048);

        HAL_Delay(20);
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
    }
}
