//
// Created by LaoZhu on 2023/5/12.
//

#ifndef POWER_MANAGER_TASKS_FSM_H_
#define POWER_MANAGER_TASKS_FSM_H_

typedef enum {
  charge_mode = 0,
  boost_mode = 1,
  normal_mode = 2,
  all_off_mode = 3
} mode_target_t;

extern TaskHandle_t fsm_task_handler;

void fsm_set_mode(mode_target_t target_mode);
void fsm_task(void *parameters);

#endif //POWER_MANAGER_TASKS_FSM_H_
