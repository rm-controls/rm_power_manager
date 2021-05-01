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
    GPIO_Config();
    SPI_Config();
    DMA_Config();
    ADC_Config();
    Referee_init();
    DAC_Config();
    UART1_Config();
    UART2_Config();
    TIM7_Config();
    Filter_Config();
    PID_ValueConfig();
    GUI_Init();
    GUI_Printf(31, 74, C_DARK_GREEN, C_WHITE, "Init Offset");
    switch (Check_ResetReason()) {
        case Power_On_Reset:GUI_Printf(21, 60, C_DARK_RED, C_WHITE, "Poweron Reset");
            break;
        case RST_Pin_Reset:GUI_Printf(21, 60, C_DARK_RED, C_WHITE, "RST Pin Reset");
            break;
        case Software_Reset:GUI_Printf(21, 60, C_DARK_RED, C_WHITE, "Software Reset");
            break;
        case IWDG_Reset:GUI_Printf(21, 60, C_DARK_RED, C_WHITE, "IWDG Reset");
            break;
        case WWDG_Reset:GUI_Printf(21, 60, C_DARK_RED, C_WHITE, "WWDG Reset");
            break;
        case LowPower_Reset:GUI_Printf(21, 60, C_DARK_RED, C_WHITE, "LowPower Reset");
            break;
        case Other_Reason:GUI_Printf(21, 60, C_DARK_RED, C_WHITE, "Other Reset");
            break;
    }
    Sensor_Config();
    memset(&FSM_Status, 0x00, sizeof(FSM_Status_t));
    Calibrate_Semaphore = xSemaphoreCreateMutex();
    xTaskCreate(Protect_Task, "ProtectTask", 512, NULL, 3, &ProtectTask_Handler);
    xTaskCreate(PID_CalculateTask, "PIDTask", 1024, NULL, 3, &PIDTask_Handler);
    xTaskCreate(FSM_Task, "FSMTask", 1024, NULL, 3, &FSMTask_Handler);
    xTaskCreate(Upload_Refree, "UploadTask", 512, NULL, 2, &UploadTask_Handler);
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
    DataRead_From_Flash();
    xTaskCreate(InitTask, "InitTask", 512, NULL, 1, &InitTask_Handler);
    vTaskStartScheduler();
    while (1);
}
