#include "main.h"

TaskHandle_t InitTask_Handler;
TaskHandle_t ReportTask_Handler;
TaskHandle_t PIDTask_Handler;
TaskHandle_t UserTask_Handler;
TaskHandle_t LEDTask_Handler;

void LED_Shine(void *pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        Delayms((unsigned int) (1000.0f / V_Capacitor));
    }
}

void InitTask(void *pvParameters) {
    taskENTER_CRITICAL();
    GPIO_Config();
    DMA_Config();
    ADC_Config();
    CAN_Config();
    DAC_Config();
    UART_Config();
    TIM_Config();
    Filter_Config();
    PID_ValueConfig();
    Sensor_Config();
    xTaskCreate(Report_Task, "ReportTask", 512, NULL, 1, &ReportTask_Handler);
    xTaskCreate(PID_CalculateTask, "PIDTask", 1024, NULL, 3, &PIDTask_Handler);
    xTaskCreate(LED_Shine, "LEDTask", 128, NULL, 1, &LEDTask_Handler);
    xTaskCreate(UserTask, "UserTask", 1024, NULL, 2, &UserTask_Handler);
    taskEXIT_CRITICAL();
    vTaskDelete(NULL);
}
int main(void) {
    HAL_Init();
    SystemClock_Config();
    xTaskCreate(InitTask, "InitTask", 512, NULL, 1, &InitTask_Handler);
    vTaskStartScheduler();
    while (1);
}
