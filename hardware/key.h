//
// Created by Lao·Zhu on 2021/3/14.
//

#ifndef POWER_MANAGER_HARDWARE_KEY_H_
#define POWER_MANAGER_HARDWARE_KEY_H_

typedef enum {
  No_Key = 0,
  Up_Key = 1,
  Down_Key = 2,
  Left_Key = 3,
  Right_Key = 4,
  Center_Key = 5,
  User_Key = 6
} Key_Num_e;

typedef struct {
  Key_Num_e Last_Num;
  Key_Num_e Num;
} Key_Press_t;

Key_Press_t ReadKey_DigitalTube();
Key_Press_t ReadKey();

#endif //POWER_MANAGER_HARDWARE_KEY_H_
