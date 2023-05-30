//
// Created by LaoZhu on 2023/5/17.
//

#include "nuc_api.h"
#include "main.h"

static unsigned char nuc_receive_status = 0;
static unsigned int last_receive_nuc_timestamp = 0;
void nuc_receive_callback(const unsigned char *buffer) {
    mode_target_e target_mode = refresh_mode;
    for (unsigned char counter = 0; counter < NUC_DMA_BUFFER_SIZE; ++counter) {
        if (buffer[counter] == 0x01) {
            nuc_receive_status = 1;
            continue;
        }
        switch (nuc_receive_status) {
            case 1:
                if (buffer[counter] == 0x00) nuc_receive_status = 2;
                else nuc_receive_status = 0;
                break;
            case 2:
                if (buffer[counter] == 0x00) nuc_receive_status = 3;
                else nuc_receive_status = 0;
                break;
            case 3:
                if (buffer[counter] == 0x39 || buffer[counter] == 0x3A) nuc_receive_status = 4;
                else nuc_receive_status = 0;
                break;
            case 4:nuc_receive_status = 0;
                switch (buffer[counter]) {
                    default:
                    case 0xE0:target_mode = normal_mode;
                        break;
                    case 0xC8:target_mode = charge_mode;
                        break;
                    case 0xCC:target_mode = boost_mode;
                        break;
                }
                break;
            default:nuc_receive_status = 0;
                break;
        }
    }
    last_receive_nuc_timestamp = HAL_GetTick();
    if (target_mode != refresh_mode)
        fsm_set_mode(target_mode);
}

unsigned int nuc_available(void) {
    return ((HAL_GetTick() - last_receive_nuc_timestamp) < 1000);
}
