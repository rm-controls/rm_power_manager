//
// Created by LaoZhu on 2023/4/10.
//

#include "power.h"
#include "system.h"
#include "stdlib.h"
#include "logic.h"
#include "verify.h"
#include "filter.h"
#include "referee.h"
#include "dma.h"

#define ADC_COEFFICIENT (3.0f / 4096.0f)

volatile unsigned char power_manager_status_buffer[UART_DMA_BUFFER_SIZE] =
    {0xA5, 0x0A, 0x00, 0x00, 0xA9, 0x01, 0x83, [7 ... (UART_DMA_BUFFER_SIZE - 1)]=0x00};
const unsigned int k_power_manager_status_buffer_length = 19;

static calibrate_params_t calibrate_params;
power_info_t power_info;

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
    unsigned short crc16_val = get_crc16_value((unsigned char *) power_manager_status_buffer, 0x8301,
                                               (unsigned char *) &power_manager_status_buffer[7], 10);
    power_manager_status_buffer[17] = crc16_val & 0x00FF;
    power_manager_status_buffer[18] = (crc16_val >> 8) & 0x00FF;
    power_manager_status_send_flag = 1;
}

int compare_ushort(const void *a, const void *b) {
    return (*(unsigned short *) a - *(unsigned short *) b);
}

void update_powerinfo(const unsigned short *adc_result) {
    power_info.charge_current =
        (float) (adc_result[0] - calibrate_params.charge_current_offset) * ADC_COEFFICIENT * 20.0f;
    power_info.chassis_current =
        (float) (adc_result[1] - calibrate_params.chassis_current_offset) * ADC_COEFFICIENT * 20.0f;
    power_info.battery_voltage = (float) adc_result[2] * ADC_COEFFICIENT * 21.0f;
    power_info.capacitor_voltage =
        get_filter_result(&capacitor_voltage_filter, (float) adc_result[3] * ADC_COEFFICIENT * 21.0f);
    power_info.chassis_voltage = (float) adc_result[4] * ADC_COEFFICIENT * 21.0f;

    float capacitor_energy = 7.5f * power_info.capacitor_voltage * power_info.capacitor_voltage - 367.5f;
    capacitor_energy = ((capacitor_energy < 0) ? 0 : capacitor_energy);
    power_info.capacitor_percent = capacitor_energy / 1434.375f;                    // Set 15.5V as 100%, 7V as 0%
    power_info.capacitor_percent = (power_info.capacitor_percent > 100.0f) ? 100.0f : power_info.capacitor_percent;

    power_info.charge_current =
        (power_info.charge_current < 0.01f || power_info.charge_current > 15.0f) ? 0 : power_info.charge_current;
    power_info.chassis_current =
        (power_info.chassis_current < 0.01f || power_info.chassis_current > 15.0f) ? 0 : power_info.chassis_current;
    power_info.battery_voltage = (power_info.battery_voltage > 35.0f) ? 0 : power_info.battery_voltage;
    power_info.capacitor_voltage = (power_info.capacitor_voltage > 25.0f) ? 0 : power_info.capacitor_voltage;
    power_info.chassis_voltage = (power_info.chassis_voltage > 35.0f) ? 0 : power_info.chassis_voltage;

    power_info.charge_power = get_filter_result(&charge_power_filter,
                                                power_info.capacitor_voltage * power_info.charge_current);
    power_info.chassis_power = get_filter_result(&chassis_power_filter,
                                                 power_info.chassis_current * power_info.chassis_voltage);
}

extern unsigned short adc_result[6];
void calibrate_params_config(void) {
    unsigned long capoffset_sum = 0, chaoffset_sum = 0;
    unsigned short adc_cap_buffer[20], adc_cha_buffer[20];
    calibrate_params.current_k = 1;
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
    if (referee_available() == 1) {
        float x[4], y[4], xy_sum = 0, x_ave, y_ave, x2_sum = 0, x_sum = 0, y_sum = 0;
        charge_switch_only();
        for (unsigned char counter = 0; counter < 4; counter++) {
            float sample_local_sum = 0, sample_referee_sum = 0;
            dac_set_output((unsigned short) (2730.0f * (float) (counter + 1)
                / power_info.capacitor_voltage));       //10 ~ 40W
            for (unsigned char counterf = 0; counterf < 5; counterf++) {
                delayms(50);
                sample_local_sum += power_info.charge_power;
                sample_referee_sum += referee_info.chassis_power;
            }
            x[counter] = sample_local_sum / 5.0f;
            y[counter] = sample_referee_sum / 5.0f;
            xy_sum += (x[counter] * y[counter]);
            x2_sum += (x[counter] * x[counter]);
            x_sum += x[counter];
            y_sum += y[counter];
        }
        close_all_switches();
        x_ave = x_sum / 4.0f;
        y_ave = y_sum / 4.0f;
        calibrate_params.current_k = (xy_sum - 4 * x_ave * y_ave) / (x2_sum - 4 * x_ave * x_ave);
        calibrate_params.current_b = y_ave - calibrate_params.current_k * x_ave;
    } else {
        calibrate_params.current_k = 1.0f;
        calibrate_params.current_b = 0;
    }
}
