#include "main.h"

TaskHandle_t InitTask_Handler;
TaskHandle_t PIDTask_Handler;
TaskHandle_t FSMTask_Handler;
TaskHandle_t UserTask_Handler;
TaskHandle_t LEDTask_Handler;
TaskHandle_t UploadTask_Handler;
TaskHandle_t ProtectTask_Handler;

void LED_Shine(void *pvParameters) {
    while (1) {
        unsigned char number = (unsigned char) V_Capacitor / 1;
        if (number / 5 >= 1) {
            while (1) {
                if (number >= 5) {
                    number = number - 5;
                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                    Delayms(500);
                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                    Delayms(500);
                } else
                    break;
            }
        }
        for (unsigned char counter = 0; counter < number * 2; counter++) {
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            Delayms(100);
        }
        Delayms(1000);
    }
}

void InitTask() {
    taskENTER_CRITICAL();
    GPIO_Config();
    DMA_Config();
    ADC_Config();
    Referee_init();
    DAC_Config();
    UART1_Config();
    UART2_Config();
    TIM7_Config();
    Filter_Config();
    PID_ValueConfig();
    Sensor_Config();
    memset(&FSM_Status, 0x00, sizeof(FSM_Status_t));
    xTaskCreate(Protect_Task, "ProtectTask", 512, NULL, 3, &ProtectTask_Handler);
    xTaskCreate(PID_CalculateTask, "PIDTask", 1024, NULL, 3, &PIDTask_Handler);
    xTaskCreate(FSM_Task, "FSMTask", 1024, NULL, 3, &FSMTask_Handler);
    xTaskCreate(Upload_Refree, "UploadTask", 512, NULL, 2, &UploadTask_Handler);
    xTaskCreate(LED_Shine, "LEDTask", 128, NULL, 1, &LEDTask_Handler);
    xTaskCreate(UserTask, "UserTask", 1024, NULL, 2, &UserTask_Handler);
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
