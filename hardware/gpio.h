#ifndef POWER_MANAGER_HARDWARE_GPIO_H_
#define POWER_MANAGER_HARDWARE_GPIO_H_

#include "stm32h7xx_hal.h"
#include "system.h"


//////////////////// Keys ////////////////////
#define UP_KEY_PIN GPIO_PIN_3
#define UP_KEY_PORT GPIOD

#define DOWN_KEY_PIN GPIO_PIN_5
#define DOWN_KEY_PORT GPIOD

#define LEFT_KEY_PIN GPIO_PIN_4
#define LEFT_KEY_PORT GPIOD

#define RIGHT_KEY_PIN GPIO_PIN_7
#define RIGHT_KEY_PORT GPIOD

#define CENTER_KEY_PIN GPIO_PIN_6
#define CENTER_KEY_PORT GPIOD


//////////////////// LCD ////////////////////
#define LCD_PWR_Pin GPIO_PIN_15
#define LCD_PWR_Port GPIOD

#define LCD_RES_Pin GPIO_PIN_0
#define LCD_RES_Port GPIOD

#define LCD_A0_Pin GPIO_PIN_1
#define LCD_A0_Port GPIOD

//////////////////// Others ////////////////////
#define CHG_EN_Pin GPIO_PIN_11
#define CHG_EN_GPIO_Port GPIOE

#define EN_NMOS_Pin GPIO_PIN_9
#define EN_NMOS_GPIO_Port GPIOE

#define BOOST_EN_Pin GPIO_PIN_10
#define BOOST_EN_GPIO_Port GPIOE

unsigned char gpio_use_lcd(void);
void gpio_config(void);

#endif /* POWER_MANAGER_HARDWARE_GPIO_H_ */
