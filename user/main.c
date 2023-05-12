#include "main.h"

void user_task(void *parameters) {
    (void) parameters;
    while (1) {
        pack_powerinfo_buffer();
        delayms(1000);
    }
}

void initialize_task(void *parameters) {
    (void) parameters;
    taskENTER_CRITICAL();
    filter_config();
    gpio_config();
    dma_config();
    ramecc_config();
    usart1_config();
    usart2_config();
//    iwdg_config();
    adc_config();
    dac_config();
    spi3_config();
    calibrate_params_config();
    calibrate_referee_config();

    BaseType_t xReturned = xTaskCreate((TaskFunction_t) user_task,
                                       (const char *) "UserTask",
                                       (configSTACK_DEPTH_TYPE) 512,
                                       (void *) NULL,
                                       (UBaseType_t) 1,
                                       (TaskHandle_t *) NULL);
    if (xReturned != pdPASS)
        error_handler(__func__, __LINE__);

    xReturned = xTaskCreate((TaskFunction_t) interrupt_handle_task,
                            (const char *) "IntTask",
                            (configSTACK_DEPTH_TYPE) 1024,
                            (void *) NULL,
                            (UBaseType_t) 4,
                            (TaskHandle_t *) NULL);
    if (xReturned != pdPASS)
        error_handler(__func__, __LINE__);

    taskEXIT_CRITICAL();
    vTaskDelete(NULL);
}

int main(void) {
    system_config();

    BaseType_t xReturned = xTaskCreate((TaskFunction_t) initialize_task,
                                       (const char *) "InitTask",
                                       (configSTACK_DEPTH_TYPE) 2048,
                                       (void *) NULL,
                                       (UBaseType_t) 1,
                                       (TaskHandle_t *) NULL);
    if (xReturned != pdPASS)
        error_handler(__func__, __LINE__);

    freertos_is_running();
    vTaskStartScheduler();
    while (1);
}
