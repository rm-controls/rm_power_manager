#include "main.h"
#include "math.h"

int main(void) {
    float theta = 0;
    HAL_Init();
    SystemClock_Config();
    GPIO_Config();
    DMA_Config();
    ADC_Config();
    CAN_Config();
    DAC_Config();
    UART_Config();
    while (1) {
        DTP_Package_t pkg = {0};
        float ADC_Resultf[4];
        theta += 0.1f;
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, ((sinf(theta) + 1.0f) * 1233.03f) / 1);
        pkg.PID = 0;
        for (unsigned char counter = 0; counter < 4; counter++)
            ADC_Resultf[counter] = (float) ADC_Result[counter] * 3.3f / 4096.0f;
        for (unsigned char counter = 0; counter < 4; counter++) {
            pkg.Data[counter * 2] = FloatToInt16(ADC_Resultf[counter]) >> 8;
            pkg.Data[counter * 2 + 1] = FloatToInt16(ADC_Resultf[counter]) & 0x00ff;
        }
        DTP_Transmit(&pkg);
        HAL_Delay(50);
    }
}
