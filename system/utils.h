//
// Created by LaoZhu on 2023/5/14.
//

#ifndef POWER_MANAGER_SYSTEM_UTILS_H_
#define POWER_MANAGER_SYSTEM_UTILS_H_

#include <stdint.h>

typedef struct {
  unsigned short year;
  unsigned short month;
  unsigned short day;
  unsigned short hour;
  unsigned short minute;
  unsigned short second;
} datetime_t;

void utc_second_to_date_time(uint64_t utc_seconds, datetime_t *datetime);

#endif //POWER_MANAGER_SYSTEM_UTILS_H_
