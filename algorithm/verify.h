//
// Created by LaoZhu on 2023/5/3.
//

#ifndef POWER_MANAGER_ALGORITHM_VERIFY_H_
#define POWER_MANAGER_ALGORITHM_VERIFY_H_

unsigned char get_crc8_value(unsigned char *buffer, unsigned int length);
unsigned short get_crc16_value(unsigned char *header, unsigned short cmd_id, unsigned char *data, unsigned int length);

#endif  // POWER_MANAGER_ALGORITHM_VERIFY_H_
