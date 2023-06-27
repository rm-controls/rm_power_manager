//
// Created by LaoZhu on 2023/5/3.
//

#ifndef POWER_MANAGER_ALGORITHM_REFEREE_H_
#define POWER_MANAGER_ALGORITHM_REFEREE_H_

typedef struct {
  float chassis_power;
  unsigned short chassis_power_buffer;
  unsigned short chassis_power_limit;
  float chassis_current;
  float chassis_voltage;
  unsigned long long timestamp;
  unsigned char game_progress;
} referee_info_t;

extern volatile referee_info_t referee_info;

unsigned int referee_available(void);
void referee_process_buffer(const unsigned char *buffer);

#endif  // POWER_MANAGER_ALGORITHM_REFEREE_H_
