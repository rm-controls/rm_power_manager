//
// Created by LaoZhu on 2023/5/3.
//

#include "referee.h"
#include "system.h"
#include "verify.h"
#include "string.h"
#include "dma.h"

static unsigned char referee_process_status = 0, referee_header_buffer[5] = {0xA5, [1 ... 4]=0};
static referee_package_t current_package = {0};

static void referee_process_struct(referee_package_t *package);

void referee_process_buffer(const unsigned char *buffer) {
    int frame_header_position = -1;
    unsigned short crc16_expected;
    for (int counter = 0; counter < UART_DMA_BUFFER_SIZE; ++counter) {
        switch (referee_process_status) {
            case 0:if (buffer[counter] == 0xA5) referee_process_status = 1;
                break;
            case 1:current_package.header.data_length = buffer[counter];
                referee_header_buffer[1] = buffer[counter];
                referee_process_status = 2;
                break;
            case 2:current_package.header.data_length |= (buffer[counter] << 8);
                current_package.header.data_length = current_package.header.data_length;
                referee_header_buffer[2] = buffer[counter];
                referee_process_status = 3;
                break;
            case 3:current_package.header.package_sequence = buffer[counter];
                referee_header_buffer[3] = buffer[counter];
                referee_process_status = 4;
                break;
            case 4:
                if (get_crc8_value(referee_header_buffer, 4) == buffer[counter]) {
                    referee_process_status = 5;
                    frame_header_position = counter - 4;
                    referee_header_buffer[4] = buffer[counter];
                } else
                    goto clear_all_referee_data;
                break;
            case 5:current_package.cmd_id = buffer[counter];
                referee_process_status = 6;
                break;
            case 6:current_package.cmd_id |= (buffer[counter] << 8);
                current_package.data_counter = 0;
                referee_process_status = 7;
                if (current_package.cmd_id >= 0x0205)
                    referee_process_status = 10;
                break;
            case 7:current_package.data[current_package.data_counter] = buffer[counter];
                current_package.data_counter++;
                if (current_package.data_counter == current_package.header.data_length)
                    referee_process_status = 8;
                break;
            case 8:current_package.crc16_low = buffer[counter];
                referee_process_status = 9;
                break;
            case 9:
                crc16_expected = get_crc16_value(referee_header_buffer,
                                                 current_package.cmd_id,
                                                 current_package.data,
                                                 current_package.header.data_length);
                if ((crc16_expected & 0xff) == current_package.crc16_low
                    && ((crc16_expected >> 8) & 0xff) == buffer[counter])
                    referee_process_struct(&current_package);
                goto clear_all_referee_data;
            case 10:current_package.data_counter++;
                if (current_package.data_counter == current_package.header.data_length + 2)
                    goto clear_all_referee_data;
                break;
            default:
            clear_all_referee_data:
                referee_process_status = 0;
                frame_header_position = -1;
                memset(&referee_header_buffer[1], 0x00, 4);
                memset(&current_package, 0x00, sizeof(referee_package_t));
                break;
        }
    }
    mdma_transmit_buffer(buffer, frame_header_position);
}

static void referee_process_struct(referee_package_t *package) {
    switch (package->cmd_id) {

    }
}
