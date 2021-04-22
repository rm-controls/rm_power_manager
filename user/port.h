//
// Created by Lao·Zhu on 2021/1/20.
//

#ifndef _USER_PORT_H_
#define _USER_PORT_H_

#define UP_KEY_PIN GPIO_PIN_3
#define DOWN_KEY_PIN GPIO_PIN_5
#define LEFT_KEY_PIN GPIO_PIN_4
#define RIGHT_KEY_PIN GPIO_PIN_7
#define CENTER_KEY_PIN GPIO_PIN_6

#define UP_KEY_PORT GPIOD
#define DOWN_KEY_PORT GPIOD
#define LEFT_KEY_PORT GPIOD
#define RIGHT_KEY_PORT GPIOD
#define CENTER_KEY_PORT GPIOD

#define LCD_RES_Pin GPIO_PIN_0
#define LCD_A0_Pin GPIO_PIN_1
#define LCD_SDA_Pin GPIO_PIN_12
#define LCD_SCL_Pin GPIO_PIN_11
#define LCD_CSB_Pin GPIO_PIN_2

#define LCD_RES_Port GPIOD
#define LCD_A0_Port GPIOD
#define LCD_SDA_Port GPIOC
#define LCD_SCL_Port GPIOC
#define LCD_CSB_Port GPIOD

#define CHG_EN_Pin GPIO_PIN_11
#define CHG_EN_GPIO_Port GPIOE

#define EN_NMOS_Pin GPIO_PIN_9
#define EN_NMOS_GPIO_Port GPIOE

#define BOOST_EN_Pin GPIO_PIN_10
#define BOOST_EN_GPIO_Port GPIOE

#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOD

#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOD

#define KEY1_Pin GPIO_PIN_8
#define KEY1_GPIO_Port GPIOB

#endif //PROGRAM_USER_PORT_H_
