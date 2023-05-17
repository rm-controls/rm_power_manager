//
// Created by LaoZhu on 2023/5/12.
//

#ifndef POWER_MANAGER_TASKS_ROUTINE_H_
#define POWER_MANAGER_TASKS_ROUTINE_H_

typedef struct {
  unsigned char over_current_flag;
  unsigned char under_voltage_flag;
  unsigned char usart1_error_flag;
  unsigned char usart2_error_flag;
} protect_item_t;

extern protect_item_t protect_info;

void protect_task(void *parameters);

#endif //POWER_MANAGER_TASKS_ROUTINE_H_
