//
// Created by LaoZhu on 2023/4/10.
//

#include "main.h"

#define ADC_COEFFICIENT (3.0f / 4096.0f)
#define ZERO_ENERGY     (7.5f * 7.5f * 7.5f)        // Set 7.5V as 0%
#define FULL_ENERGY     (7.5f * 15.7f * 15.7f)      // Set 15.7V as 100%

volatile unsigned char power_manager_status_buffer[19] =
    {0xA5, 0x0A, 0x00, 0x00, 0xA9, 0x01, 0x83, [7 ... 18]=0x00};
const unsigned int k_power_manager_status_buffer_length = 19;

volatile calibrate_params_t calibrate_params;
volatile power_info_t power_info;

void pack_powerinfo_buffer() {
    unsigned short chassis_power = (unsigned short) (power_info.chassis_power * 100.0f);
    unsigned short expect_chassis_power = (unsigned short) (power_info.expect_chassis_power * 100.0f);
    unsigned short charge_power = (unsigned short) (power_info.charge_power * 100.0f);
    unsigned short remain_power = (unsigned short) (power_info.capacitor_percent * 100.0f);
    power_manager_status_buffer[7] = ((chassis_power >> 8) & 0x00FF);
    power_manager_status_buffer[8] = (chassis_power & 0x00FF);
    power_manager_status_buffer[9] = ((expect_chassis_power >> 8) & 0x00FF);
    power_manager_status_buffer[10] = (expect_chassis_power & 0x00FF);
    power_manager_status_buffer[11] = ((charge_power >> 8) & 0x00FF);
    power_manager_status_buffer[12] = (charge_power & 0x00FF);
    power_manager_status_buffer[13] = ((remain_power >> 8) & 0x00FF);
    power_manager_status_buffer[14] = (remain_power & 0x00FF);
    power_manager_status_buffer[15] = pid_get_expect();
    power_manager_status_buffer[16] =
        ((fsm_get_mode() & 0x0F) << 4) | ((protect_get_info() & 0x03) << 2) | (fsm_get_typology() & 0x03);
    unsigned short crc16_val = get_crc16_value((unsigned char *) power_manager_status_buffer, 0x8301,
                                               (unsigned char *) &power_manager_status_buffer[7], 10);
    power_manager_status_buffer[17] = crc16_val & 0x00FF;
    power_manager_status_buffer[18] = (crc16_val >> 8) & 0x00FF;
    if (protect_get_info() != 3)
        power_manager_status_send_flag = 1;
    else {
        power_manager_status_send_flag = 2;
        mdma_transmit_buffer((unsigned char *) power_manager_status_buffer, -1);
    }
}

int compare_ushort(const void *a, const void *b) {
    return (*(unsigned short *) a - *(unsigned short *) b);
}

void update_powerinfo(const unsigned short *adc_result) {
    power_info.charge_current =
        (float) (adc_result[0] - calibrate_params.charge_current_offset) * ADC_COEFFICIENT * 5.0f;
    power_info.chassis_current =
        (float) (adc_result[1] - calibrate_params.chassis_current_offset) * ADC_COEFFICIENT * 5.0f;
    power_info.battery_voltage = (float) adc_result[2] * ADC_COEFFICIENT * 11.0f;
    power_info.capacitor_voltage =
        get_filter_result(&capacitor_voltage_filter, (float) adc_result[3] * ADC_COEFFICIENT * 6.60f);
    power_info.chassis_voltage = (float) adc_result[4] * ADC_COEFFICIENT * 11.0f;

    float capacitor_energy = 7.5f * power_info.capacitor_voltage * power_info.capacitor_voltage - ZERO_ENERGY;
    capacitor_energy = ((capacitor_energy < 0) ? 0 : capacitor_energy);
    power_info.capacitor_percent = capacitor_energy * 100.0f / (FULL_ENERGY - ZERO_ENERGY);
    power_info.capacitor_percent = (power_info.capacitor_percent > 100.0f) ? 100.0f : power_info.capacitor_percent;

    power_info.charge_current =
        (power_info.charge_current < 0.01f || power_info.charge_current > 20.0f) ? 0 : power_info.charge_current;
    power_info.chassis_current =
        (power_info.chassis_current < 0.01f || power_info.chassis_current > 20.0f) ? 0 : power_info.chassis_current;
    power_info.battery_voltage = (power_info.battery_voltage > 35.0f) ? 0 : power_info.battery_voltage;
    power_info.capacitor_voltage = (power_info.capacitor_voltage > 25.0f) ? 0 : power_info.capacitor_voltage;
    power_info.chassis_voltage = (power_info.chassis_voltage > 35.0f) ? 0 : power_info.chassis_voltage;

    power_info.charge_power = get_filter_result(&charge_power_filter,
                                                power_info.battery_voltage * power_info.charge_current);
    power_info.charge_power = power_info.charge_power * calibrate_params.current_k + calibrate_params.current_b;
    power_info.chassis_power = get_filter_result(&chassis_power_filter,
                                                 power_info.chassis_current * power_info.chassis_voltage);

    power_info.charge_power = power_info.charge_power < 0 ? 0 : power_info.charge_power;
}

extern unsigned short adc_result[6];
void calibrate_params_config(void) {
    unsigned long capoffset_sum = 0, chaoffset_sum = 0;
    unsigned short adc_cap_buffer[20], adc_cha_buffer[20];
    calibrate_params.current_k = 1.0f;
    calibrate_params.current_b = 0;
    close_all_switches();
    delayms(200);
    for (unsigned char counter = 0; counter < 20; counter++) {
        adc_cap_buffer[counter] = adc_result[0];
        adc_cha_buffer[counter] = adc_result[1];
        delayms(20);
    }
    qsort(adc_cap_buffer, 20, sizeof(adc_cap_buffer[0]), compare_ushort);
    qsort(adc_cha_buffer, 20, sizeof(adc_cha_buffer[0]), compare_ushort);
    for (unsigned char counter = 0; counter < 10; counter++) {
        capoffset_sum += adc_cap_buffer[5 + counter];
        chaoffset_sum += adc_cha_buffer[5 + counter];
    }
    calibrate_params.charge_current_offset = capoffset_sum / 10;
    calibrate_params.chassis_current_offset = chaoffset_sum / 10;
}

void calibrate_referee_config(void) {              // Least square fitting of first order function
    HAL_PWR_EnableBkUpAccess();
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == 0X83838384) {
        unsigned int param_k = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
        unsigned int param_b = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
        calibrate_params.current_k = *((float *) &param_k);
        calibrate_params.current_b = *((float *) &param_b);
        goto check_params_available;
    }

    if (referee_available() == 1) {
        float x[5], y[5], xy_sum = 0, x_ave, y_ave, x2_sum = 0, x_sum = 0, y_sum = 0;
        charge_switch_only();
        calibrate_params.current_k = 1.0f;
        calibrate_params.current_b = 0;
        x[0] = 0;
        y[0] = 0;
        while (1) {
            if (power_info.capacitor_voltage < 4.0f)
                dac_set_output(2730);
            else
                break;
            delayms(100);
        }
        for (unsigned char counter = 1; counter < 5; counter++) {
            float sample_local_sum = 0, sample_referee_sum = 0;
            dac_set_output((unsigned short) (2730.0f * (float) (counter) / power_info.capacitor_voltage));
            for (unsigned char counterf = 0; counterf < 2; counterf++) {
                delayms(125);
                sample_local_sum += power_info.charge_power;
                sample_referee_sum += referee_info.chassis_power;
            }
            x[counter] = sample_local_sum / 2.0f;
            y[counter] = sample_referee_sum / 2.0f;
            xy_sum += (x[counter] * y[counter]);
            x2_sum += (x[counter] * x[counter]);
            x_sum += x[counter];
            y_sum += y[counter];
        }
        close_all_switches();
        x_ave = x_sum / 5.0f;
        y_ave = y_sum / 5.0f;
        calibrate_params.current_k = (xy_sum - 4 * x_ave * y_ave) / (x2_sum - 4 * x_ave * x_ave);
        calibrate_params.current_b = y_ave - calibrate_params.current_k * x_ave;
    } else {
        calibrate_params.current_k = 1.2f;
        calibrate_params.current_b = 0;
    }

    check_params_available:
    if (calibrate_params.current_k > 1.3f || calibrate_params.current_k < 0.9f) {
        calibrate_params.current_k = 1.2f;
        calibrate_params.current_b = 0;
    }
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0X5A5A5A5A);
}
