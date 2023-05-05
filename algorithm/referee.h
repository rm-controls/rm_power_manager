//
// Created by LaoZhu on 2023/5/3.
//

#ifndef POWER_MANAGER_ALGORITHM_REFEREE_H_
#define POWER_MANAGER_ALGORITHM_REFEREE_H_

typedef struct {
  unsigned char dummy_byte;
  unsigned char package_sequence;
  unsigned short data_length;
} referee_package_header_t;

typedef struct {
  referee_package_header_t header;
  unsigned short cmd_id;
  unsigned short data_counter;
  unsigned char data[127];
  unsigned char crc16_low;
} referee_package_t;

void referee_process_buffer(const unsigned char *buffer);

#endif //POWER_MANAGER_ALGORITHM_REFEREE_H_
