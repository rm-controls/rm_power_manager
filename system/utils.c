//
// Created by LaoZhu on 2023/5/14.
//

#include "utils.h"

static unsigned char days_in_month[12] = {31, 28, 31, 30, 31, 30,
                                          31, 31, 30, 31, 30, 31};

void utc_second_to_date_time(uint64_t utc_seconds, datetime_t *datetime) {
    uint64_t days_since_epoch, remainder;

    // Calculate days since epoch (1970-01-01), beijing time
    utc_seconds = utc_seconds + 8 * 3600;
    days_since_epoch = utc_seconds / 86400;
    remainder = utc_seconds % 86400;

    // Calculate time of day
    datetime->hour = remainder / 3600;
    remainder = remainder % 3600;
    datetime->minute = remainder / 60;
    datetime->second = remainder % 60;

    // Calculate year, month, and day
    datetime->year = 1970;
    while (days_since_epoch >= 365) {
        if ((datetime->year % 4 == 0 && datetime->year % 100 != 0) || datetime->year % 400 == 0) {
            if (days_since_epoch >= 366) {
                days_since_epoch -= 366;
                datetime->year++;
            } else
                break;
        } else {
            days_since_epoch -= 365;
            datetime->year++;
        }
    }

    if ((datetime->year % 4 == 0 && datetime->year % 100 != 0) || datetime->year % 400 == 0)
        days_in_month[1] = 29;

    datetime->month = 1;
    while (days_since_epoch >= days_in_month[datetime->month - 1]) {
        days_since_epoch -= days_in_month[datetime->month - 1];
        datetime->month++;
    }

    datetime->day = days_since_epoch + 1; // Add 1 to get 1-based day of month
}