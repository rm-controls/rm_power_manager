#include "main.h"

unsigned char lcd_digital_tube_check(unsigned int delay_xms) {
    BaseType_t xReturned;
    unsigned char using_lcd_flag = gpio_use_lcd();
    HAL_IWDG_Refresh(&hiwdg1);
    if (using_lcd_flag == 1) {
        xReturned = xTaskCreate((TaskFunction_t) gui_task,
                                (const char *) "GUITask",
                                (configSTACK_DEPTH_TYPE) 2048,
                                (void *) NULL,
                                (UBaseType_t) 1,
                                (TaskHandle_t *) NULL);
        if (xReturned != pdPASS)
            error_handler(__FILE__, __LINE__);
    } else {
        xReturned = xTaskCreate((TaskFunction_t) digital_tube_task,
                                (const char *) "TubeTask",
                                (configSTACK_DEPTH_TYPE) 2048,
                                (void *) NULL,
                                (UBaseType_t) 1,
                                (TaskHandle_t *) NULL);
        if (xReturned != pdPASS)
            error_handler(__FILE__, __LINE__);
    }
    delayms(delay_xms);
    return using_lcd_flag;
}

void initialize_task(void *parameters) {
    (void) parameters;
    BaseType_t xReturned;
    rtc_config();
    filter_config();
    dma_config();
    ramecc_config();
    usart1_config();
    usart2_config();
    error_check();
    adc_config();
    dac_config();
    spi1_config();
    spi3_config();
    w25qxx_config();
    filesystem_config();
    calibrate_params_config();
    iwdg_config();

    lcd_digital_tube_check(1);

    xReturned = xTaskCreate((TaskFunction_t) fsm_task,
                            (const char *) "FSMTask",
                            (configSTACK_DEPTH_TYPE) 1024,
                            (void *) NULL,
                            (UBaseType_t) 2,
                            (TaskHandle_t *) &fsm_task_handler);
    if (xReturned != pdPASS)
        error_handler(__FILE__, __LINE__);

    xReturned = xTaskCreate((TaskFunction_t) protect_task,
                            (const char *) "ProtectTask",
                            (configSTACK_DEPTH_TYPE) 1024,
                            (void *) NULL,
                            (UBaseType_t) 3,
                            (TaskHandle_t *) NULL);
    if (xReturned != pdPASS)
        error_handler(__FILE__, __LINE__);

    xReturned = xTaskCreate((TaskFunction_t) interrupt_handle_task,
                            (const char *) "IntTask",
                            (configSTACK_DEPTH_TYPE) 1024,
                            (void *) NULL,
                            (UBaseType_t) 4,
                            (TaskHandle_t *) NULL);
    if (xReturned != pdPASS)
        error_handler(__FILE__, __LINE__);

    vTaskDelete(NULL);
}

int main(void) {
    system_config();
    gpio_config();
    tim7_config();

    BaseType_t xReturned = xTaskCreate((TaskFunction_t) initialize_task,
                                       (const char *) "InitTask",
                                       (configSTACK_DEPTH_TYPE) 2048,
                                       (void *) NULL,
                                       (UBaseType_t) 1,
                                       (TaskHandle_t *) NULL);
    if (xReturned != pdPASS)
        error_handler(__FILE__, __LINE__);

    freertos_is_running();
    vTaskStartScheduler();
    while (1);
}
