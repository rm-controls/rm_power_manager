#include "gpio.h"

/** Configure pins
     PH0-OSC_IN (PH0)   ------> RCC_OSC_IN
     PH1-OSC_OUT (PH1)   ------> RCC_OSC_OUT
     PA13 (JTMS/SWDIO)   ------> DEBUG_JTMS-SWDIO
     PA14 (JTCK/SWCLK)   ------> DEBUG_JTCK-SWCLK
*/

void gpio_config(void) {
    GPIO_InitTypeDef GPIO_Structure;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_Structure.Pin = LCD_PWR_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(LCD_PWR_Port, &GPIO_Structure);

    GPIO_Structure.Pin = LCD_RES_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(LCD_RES_Port, &GPIO_Structure);

    GPIO_Structure.Pin = LCD_A0_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(LCD_A0_Port, &GPIO_Structure);

    GPIO_Structure.Pin = UP_KEY_PIN;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_INPUT;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(UP_KEY_PORT, &GPIO_Structure);

    GPIO_Structure.Pin = DOWN_KEY_PIN;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_INPUT;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DOWN_KEY_PORT, &GPIO_Structure);

    GPIO_Structure.Pin = LEFT_KEY_PIN;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_INPUT;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(LEFT_KEY_PORT, &GPIO_Structure);

    GPIO_Structure.Pin = RIGHT_KEY_PIN;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_INPUT;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(RIGHT_KEY_PORT, &GPIO_Structure);

    GPIO_Structure.Pin = CENTER_KEY_PIN;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_INPUT;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(CENTER_KEY_PORT, &GPIO_Structure);

    GPIO_Structure.Pin = CHG_EN_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(CHG_EN_GPIO_Port, &GPIO_Structure);

    GPIO_Structure.Pin = BOOST_EN_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BOOST_EN_GPIO_Port, &GPIO_Structure);

    GPIO_Structure.Pin = EN_NMOS_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(EN_NMOS_GPIO_Port, &GPIO_Structure);

    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_PWR_Port, LCD_PWR_Pin, GPIO_PIN_SET);
}
