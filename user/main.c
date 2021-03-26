#include "main.h"

TaskHandle_t InitTask_Handler;
TaskHandle_t ReportTask_Handler;
TaskHandle_t PIDTask_Handler;
TaskHandle_t UserTask_Handler;
TaskHandle_t LEDTask_Handler;

void LED_Shine(void *pvParameters) {
    while (1) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//        unsigned char number = (unsigned char) V_Capacitor / 1;
//        for (unsigned char counter = 0; counter < number * 2; counter++) {
//            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//            Delayms((unsigned int) (400 / number));
//        }
        Delayms(1200);
    }
}

void InitTask(void *pvParameters) {
    taskENTER_CRITICAL();
//    CRC_Config();
    GPIO_Config();
    DMA_Config();
    ADC_Config();
    DAC_Config();
    UART1_Config();
    UART2_Config();
//    Filter_Config();
//    PID_ValueConfig();
//    Sensor_Config();
//    xTaskCreate(Report_Task, "ReportTask", 512, NULL, 1, &ReportTask_Handler);
//    xTaskCreate(PID_CalculateTask, "PIDTask", 1024, NULL, 3, &PIDTask_Handler);
    xTaskCreate(LED_Shine, "LEDTask", 128, NULL, 1, &LEDTask_Handler);
//    xTaskCreate(UserTask, "UserTask", 1024, NULL, 2, &UserTask_Handler);
    taskEXIT_CRITICAL();
    vTaskDelete(NULL);
}
int main(void) {
    MPU_Config();
    Cache_Config();
    HAL_Init();
    SystemClock_Config();
    xTaskCreate(InitTask, "InitTask", 512, NULL, 1, &InitTask_Handler);
    vTaskStartScheduler();
    while (1);
}
