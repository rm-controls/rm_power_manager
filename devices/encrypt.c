#include "encrypt.h"
#include "string.h"
#include "stm32h7xx_hal.h"
#include "usart.h"

void DTP_Transmit(DTP_Package_t *dtp_package) {
    unsigned char temp_buf[12] = {0};
    unsigned char cnt;
    memset(temp_buf, 0x00, sizeof(temp_buf));
    temp_buf[0] = 0xff;                                         //set start of frame
    temp_buf[10] = 0x00;                                        //initialize adjust frame
    for (cnt = 0; cnt < 8; cnt++)                               //copy data and adjust
    {
        if (dtp_package->Data[cnt] == 0xff)                     //if data equal 0xff, mark it
        {
            temp_buf[2 + cnt] = 0x00;
            temp_buf[10] = (temp_buf[10] | (1 << cnt));
        } else
            temp_buf[2 + cnt] = dtp_package->Data[cnt];
    }
    temp_buf[1] = dtp_package->PID << 4 | ((~dtp_package->PID) & 0x0f);     //set pid and check
    temp_buf[11] = 0xff;                                                    //set end of frame
//    HAL_UART_Transmit(&huart1, temp_buf, sizeof(temp_buf), 0xFFFFFFFFUL);
}
