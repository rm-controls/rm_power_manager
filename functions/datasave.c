//
// Created by Lao·Zhu on 2021/4/6.
//

#include "main.h"

unsigned char Reset_Number = 0;

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    DataSave_To_Flash(StackOverflow_Reset);
}

void DataSave_DisplayLastInfo(void) {
    FSM_Status_t FSM_Before_Reboot;
    float Last_I_Capacitor, Last_I_Chassis, Last_V_Capacitor;
    float Last_V_Chassis, Last_V_Baterry, Last_RefreeChassis_Power;
    unsigned char Buffer[16], reason, Last_Power_Limit;
    unsigned int LongBuffer[4];
    HAL_PWR_EnableBkUpAccess();
    for (unsigned char counter = 0; counter < 4; ++counter)
        LongBuffer[counter] = HAL_RTCEx_BKUPRead(&hrtc, counter);
    for (unsigned char counter = 0; counter < 16; counter++)
        Buffer[counter] = LongBuffer[counter / 4] >> (8 * (counter % 4));
    Last_I_Capacitor = Int16ToFloat(Buffer[2] << 8 | Buffer[3]);
    Last_I_Chassis = Int16ToFloat(Buffer[4] << 8 | Buffer[5]);
    Last_V_Capacitor = Int16ToFloat(Buffer[6] << 8 | Buffer[7]);
    Last_V_Chassis = Int16ToFloat(Buffer[8] << 8 | Buffer[9]);
    Last_V_Baterry = Int16ToFloat(Buffer[10] << 8 | Buffer[11]);
    Last_RefreeChassis_Power = Int16ToFloat(Buffer[13] << 8 | Buffer[14]);
    reason = (Buffer[0] >> 5) & 0x07;
    FSM_Before_Reboot.Charge_Mode = (Buffer[0] >> 2) & 0x07;
    FSM_Before_Reboot.Typology_Mode = Buffer[0] & 0x03;
    FSM_Before_Reboot.FSM_Mode = (Buffer[1] >> 2) & 0x07;
    Last_Power_Limit = Buffer[12];

    GUI_Clear(C_WHITE);
    GUI_Printf(22, 2, C_DARK_RED, C_WHITE,
               "%-2d.%1dA",
               (int) Last_I_Capacitor,
               (int) (Last_I_Capacitor * 10) - ((int) Last_I_Capacitor * 10));
    GUI_Printf(74, 2, C_DARK_RED, C_WHITE,
               "%-2d.%1dA", (int) Last_I_Chassis,
               (int) (Last_I_Chassis * 10) - ((int) Last_I_Chassis * 10));
    GUI_Printf(9, 16, C_DARK_RED, C_WHITE,
               "%-2d.%1dV", (int) Last_V_Capacitor,
               (int) (Last_V_Capacitor * 10) - ((int) Last_V_Capacitor * 10));
    GUI_Printf(48, 16, C_DARK_RED, C_WHITE,
               "%-2d.%1dV", (int) Last_V_Chassis,
               (int) (Last_V_Chassis * 10) - ((int) Last_V_Chassis * 10));
    GUI_Printf(87, 16, C_DARK_RED, C_WHITE,
               "%-2d.%1dV", (int) Last_V_Baterry,
               (int) (Last_V_Baterry * 10) - ((int) Last_V_Baterry * 10));
    switch (FSM_Before_Reboot.FSM_Mode) {
        case Normal_Mode:GUI_Printf(2, 30, C_DARK_RED, C_WHITE, "Normal");
            break;
        case OverPower_Mode:GUI_Printf(2, 30, C_DARK_RED, C_WHITE, "OverPower");
            break;
        case Halt_Mode:GUI_Printf(2, 30, C_DARK_RED, C_WHITE, "Halt");
            break;
        case Transition_Mode:GUI_Printf(2, 30, C_DARK_RED, C_WHITE, "Transit");
            break;
    }
    switch (FSM_Before_Reboot.Charge_Mode) {
        case Zero_Power_Charge:GUI_Printf(60, 30, C_DARK_RED, C_WHITE, "ZeroPower");
            break;
        case Const_Power_Charge:GUI_Printf(60, 30, C_DARK_RED, C_WHITE, "ConstPower");
            break;
        case Proportional_Charge:GUI_Printf(60, 30, C_DARK_RED, C_WHITE, "Proportional");
            break;
        case Full_Power_Charge:GUI_Printf(60, 30, C_DARK_RED, C_WHITE, "FullPower");
            break;
        case Remain_Power_Charge:GUI_Printf(60, 30, C_DARK_RED, C_WHITE, "RemainPower");
            break;
    }
    switch (FSM_Before_Reboot.Typology_Mode) {
        case Only_Charge:GUI_Printf(10, 44, C_DARK_RED, C_WHITE, "OnlyCharge");
            break;
        case Only_PMOS:GUI_Printf(10, 44, C_DARK_RED, C_WHITE, "OnlyPMOS");
            break;
        case PMOS_With_Charge:GUI_Printf(10, 44, C_DARK_RED, C_WHITE, "PMOS&Charge");
            break;
        case All_Off:GUI_Printf(10, 44, C_DARK_RED, C_WHITE, "AllOff");
            break;
    }
    GUI_Printf(80, 44, C_DARK_RED, C_WHITE,
               "%-3d.%1dW", (int) Last_RefreeChassis_Power,
               (int) (Last_RefreeChassis_Power * 10) - ((int) Last_RefreeChassis_Power * 10));
    switch (reason) {
        case RePowerOn_Reset:GUI_Printf(2, 60, C_RED, C_WHITE, "Repoweron Reset");
            break;
        case Watchdog_Reset:GUI_Printf(2, 60, C_RED, C_WHITE, "WatchDog Reset");
            break;
        case Hardfault_Reset:GUI_Printf(2, 60, C_RED, C_WHITE, "HardFault Reset");
            break;
        case UsageFault_Reset:GUI_Printf(2, 60, C_RED, C_WHITE, "UsageFault Reset");
            break;
        case BusFault_Reset:GUI_Printf(2, 60, C_RED, C_WHITE, "BusFault Reset");
            break;
        case NMI_Reset:GUI_Printf(2, 60, C_RED, C_WHITE, "NMI Reset");
            break;
        case StackOverflow_Reset:GUI_Printf(2, 60, C_RED, C_WHITE, "Stack Overflow Reset");
            break;
        default:break;
    }
    GUI_Printf(16, 74, C_DARK_RED, C_WHITE, "LimitPower: %-3dW", Last_Power_Limit);
    if ((Buffer[15] & 0x80) != 0)
        GUI_Printf(2, 88, C_DARK_RED, C_WHITE, "EN_NMOS: OFF");
    else
        GUI_Printf(2, 88, C_DARK_RED, C_WHITE, "EN_NMOS: ON");
    if ((Buffer[15] & 0x40) != 0)
        GUI_Printf(2, 102, C_DARK_RED, C_WHITE, "EN_CHG: OFF");
    else
        GUI_Printf(2, 102, C_DARK_RED, C_WHITE, "EN_CHG: ON");
    if ((Buffer[15] & 0x20) != 0)
        GUI_Printf(2, 116, C_DARK_RED, C_WHITE, "EN_BOOST: OFF");
    else
        GUI_Printf(2, 116, C_DARK_RED, C_WHITE, "EN_BOOST: ON");
    while (1);
}

void DataSave_To_Flash(Saving_Reason_e reason) {
    unsigned char Buffer[16];
    unsigned int ulOriginalBASEPRI;
    if (reason == RePowerOn_Reset || reason == StackOverflow_Reset)
        taskENTER_CRITICAL();
    else
        ulOriginalBASEPRI = taskENTER_CRITICAL_FROM_ISR();
    __HAL_RCC_WWDG1_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(WWDG_IRQn);

    Buffer[0] = (reason & 0x07) << 5;
    Buffer[0] |= (FSM_Status.Charge_Mode & 0x07) << 2;
    Buffer[0] |= FSM_Status.Typology_Mode & 0x03;
    Buffer[1] = (FSM_Status.Expect_Mode & 0x07) << 5;
    Buffer[1] |= (FSM_Status.FSM_Mode & 0x07) << 2;
    Buffer[1] |= Reset_Number & 0x03;
    Buffer[2] = FloatToInt16(I_Capacitor) >> 8;
    Buffer[3] = FloatToInt16(I_Capacitor) & 0x00ff;
    Buffer[4] = FloatToInt16(I_Chassis) >> 8;
    Buffer[5] = FloatToInt16(I_Chassis) & 0x00ff;
    Buffer[6] = FloatToInt16(V_Capacitor) >> 8;
    Buffer[7] = FloatToInt16(V_Capacitor) & 0x00ff;
    Buffer[8] = FloatToInt16(V_Chassis) >> 8;
    Buffer[9] = FloatToInt16(V_Chassis) & 0x00ff;
    Buffer[10] = FloatToInt16(V_Baterry) >> 8;
    Buffer[11] = FloatToInt16(V_Baterry) & 0x00ff;
    Buffer[12] = referee_data_.game_robot_status_.chassis_power_limit;
    Buffer[13] = FloatToInt16(referee_data_.power_heat_data_.chassis_power) >> 8;
    Buffer[14] = FloatToInt16(referee_data_.power_heat_data_.chassis_power) & 0x00ff;
    Buffer[15] = ((HAL_GPIO_ReadPin(EN_NMOS_GPIO_Port, EN_NMOS_Pin) & 0x01) << 7)
        | ((HAL_GPIO_ReadPin(CHG_EN_GPIO_Port, CHG_EN_Pin) & 0x01) << 6)
        | ((HAL_GPIO_ReadPin(BOOST_EN_GPIO_Port, BOOST_EN_Pin) & 0x01) << 5);

    HAL_PWR_EnableBkUpAccess();
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, Buffer[0] | Buffer[1] << 8 | Buffer[2] << 16 | Buffer[3] << 24);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, Buffer[4] | Buffer[5] << 8 | Buffer[6] << 16 | Buffer[7] << 24);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, Buffer[8] | Buffer[9] << 8 | Buffer[10] << 16 | Buffer[11] << 24);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, Buffer[12] | Buffer[13] << 8 | Buffer[14] << 16 | Buffer[15] << 24);

    GUI_Clear(C_WHITE);
    switch (reason) {
        case RePowerOn_Reset:GUI_Printf(3, 4, C_BLACK, C_WHITE, "Repoweron Reset");
            break;
        case Watchdog_Reset:GUI_Printf(3, 4, C_BLACK, C_WHITE, "WatchDog Reset");
            break;
        case Hardfault_Reset:GUI_Printf(3, 4, C_BLACK, C_WHITE, "HardFault Reset");
            break;
        case UsageFault_Reset:GUI_Printf(3, 4, C_BLACK, C_WHITE, "UsageFault Reset");
            break;
        case BusFault_Reset:GUI_Printf(3, 4, C_BLACK, C_WHITE, "BusFault Reset");
            break;
        case NMI_Reset:GUI_Printf(3, 4, C_BLACK, C_WHITE, "NMI Reset");
            break;
        case StackOverflow_Reset:GUI_Printf(3, 4, C_BLACK, C_WHITE, "Stack Overflow Reset");
            break;
        default:break;
    }

    for (unsigned char counter = 0; counter < 16; counter++)
        CurrentFile_Structure->ResetInfo[counter] = Buffer[counter];
    FileSystem_WriteIntoFlash();
    HAL_Delay(500);
    if (reason == RePowerOn_Reset || reason == StackOverflow_Reset)
        taskEXIT_CRITICAL();
    else
        taskEXIT_CRITICAL_FROM_ISR(ulOriginalBASEPRI);
}
