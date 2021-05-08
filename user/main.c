#include "main.h"

TaskHandle_t InitTask_Handler;
TaskHandle_t PIDTask_Handler;
TaskHandle_t FSMTask_Handler;
TaskHandle_t UserTask_Handler;
TaskHandle_t LCDTask_Handler;
TaskHandle_t UploadTask_Handler;
TaskHandle_t ProtectTask_Handler;
SemaphoreHandle_t Calibrate_Semaphore;

void InitTask() {
    taskENTER_CRITICAL();
#if USE_RTC_ONCHIP == 1
    Delayms(1000);
    RTC_Config();
#endif
    GPIO_Config();
    GUI_Init();
    GUI_Printf(31, 74, C_DARK_GREEN, C_WHITE, "Init Offset");
#if USE_SPI_FLASH_FATFS == 1
    SPI_Config();
    W25QXX_Init();
    FileSystem_Config();
#endif
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
    Calibrate_Semaphore = xSemaphoreCreateMutex();
    xTaskCreate(Protect_Task, "ProtectTask", 1024, NULL, 3, &ProtectTask_Handler);
    xTaskCreate(PID_CalculateTask, "PIDTask", 1024, NULL, 3, &PIDTask_Handler);
    xTaskCreate(FSM_Task, "FSMTask", 2048, NULL, 3, &FSMTask_Handler);
    xTaskCreate(Upload_Refree, "UploadTask", 1024, NULL, 2, &UploadTask_Handler);
    xTaskCreate(LCD_Refresh, "LCDTask", 10240, NULL, 1, &LCDTask_Handler);
    xTaskCreate(UserTask, "UserTask", 1024, NULL, 2, &UserTask_Handler);
    taskEXIT_CRITICAL();
    vTaskDelete(NULL);
}

int main(void) {
    MPU_Config();
    Cache_Config();
    HAL_Init();
    SystemClock_Config();
    xTaskCreate(InitTask, "InitTask", 2048, NULL, 1, &InitTask_Handler);
    vTaskStartScheduler();
    while (1);
}
