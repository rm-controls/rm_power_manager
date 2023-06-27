//
// Created by LaoZhu on 2023/4/10.
//

#include "logic.h"
#include "gpio.h"
#include "dac.h"

void dac_set_output(unsigned short value) {
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
}

void close_all_switches(void) {
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x00);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
}

void charge_switch_only(void) {
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x00);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
}

void passthrough_switch_only(unsigned char is_last_boost) {
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_RESET);
    if (is_last_boost)
        delayms(50);
    HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_SET);
}

void charge_with_boost_switches(unsigned char is_last_pass, unsigned char is_over_voltage) {
    if (is_over_voltage)
        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(CHG_EN_GPIO_Port, CHG_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BOOST_EN_GPIO_Port, BOOST_EN_Pin, GPIO_PIN_RESET);
    if (is_last_pass)
        delayms(50);
    HAL_GPIO_WritePin(EN_NMOS_GPIO_Port, EN_NMOS_Pin, GPIO_PIN_SET);
}

void hardware_wdi_toggle(void) {
    HAL_GPIO_TogglePin(HARDWARE_IWD_GPIO_Port, HARDWARE_IWD_Pin);
}