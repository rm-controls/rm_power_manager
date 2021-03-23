#include "gpio.h"
#include "port.h"
/** Configure pins
     PH0-OSC_IN (PH0)   ------> RCC_OSC_IN
     PH1-OSC_OUT (PH1)   ------> RCC_OSC_OUT
     PA13 (JTMS/SWDIO)   ------> DEBUG_JTMS-SWDIO
     PA14 (JTCK/SWCLK)   ------> DEBUG_JTCK-SWCLK
*/
void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_Structure;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_Structure.Pin = LED1_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_Structure);

    GPIO_Structure.Pin = LED2_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_Structure);

    GPIO_Structure.Pin = KEY1_Pin;
    GPIO_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Structure.Mode = GPIO_MODE_INPUT;
    GPIO_Structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_Structure);

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

    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
}
