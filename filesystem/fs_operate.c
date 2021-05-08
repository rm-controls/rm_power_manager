//
// Created by Lao·Zhu on 2021/5/3.
//

#include "gui_base.h"
#include "fs_operate.h"
#include "FreeRTOS.h"
#include "flash.h"
#include "rtc.h"

FileSystem_Struct_t *FileSystem_Structure;
File_Struct_t *CurrentFile_Structure = NULL;
FileHead_Struct_t *LastFileHead_Structure = NULL;
unsigned int CurrentFile_Address = 0, LastFile_Address = 0;

void FileSystem_ReadItem(unsigned char itemnum) {
    unsigned int NextHeadAddr = FileSystem_Structure->FirstFileAddr;
    FileHead_Struct_t *CurrentFile_Head_Tmp = pvPortMalloc(sizeof(FileHead_Struct_t));
    for (unsigned char counter = 0; counter < itemnum; counter++) {
        W25QXX_Read((unsigned char *) CurrentFile_Head_Tmp, NextHeadAddr, sizeof(FileHead_Struct_t));
        NextHeadAddr = CurrentFile_Head_Tmp->NextFileAddr;
    }
    vPortFree(CurrentFile_Head_Tmp);
}

void FileSystem_WriteIntoFlash(void) {
    if (LastFileHead_Structure != NULL && LastFile_Address != 0) {
        W25QXX_Write((unsigned char *) LastFileHead_Structure, LastFile_Address, sizeof(FileHead_Struct_t));
    }
    W25QXX_Write((unsigned char *) CurrentFile_Structure, CurrentFile_Address, sizeof(File_Struct_t));
    W25QXX_Write((unsigned char *) FileSystem_Structure, 0x00000, sizeof(FileSystem_Struct_t));
}

void FileSystem_FormatFlash(void) {
    for (unsigned int counter = 0; counter < sizeof(FileSystem_Struct_t); counter++)
        ((unsigned char *) FileSystem_Structure)[counter] = 0x00;
    W25QXX_WAKEUP();
    W25QXX_Write_Enable();
    for (unsigned int counter = 0; counter < ((sizeof(FileSystem_Struct_t) + 256 * sizeof(File_Struct_t)) / 4096) + 1;
         counter++)
        W25QXX_Erase_Sector(counter);
    FileSystem_Structure->FSHead = 0xA5;
    FileSystem_Structure->FileNum = 0;
    W25QXX_Write((unsigned char *) FileSystem_Structure, 0x00000, sizeof(FileSystem_Struct_t));
}

void FileSystem_FindRemainSpace(void) {
    if (FileSystem_Structure->FileNum != 255) {
        for (unsigned short counter = 0; counter < 256; counter++) {
            unsigned char OperateLUE = FileSystem_Structure->FileLUT[counter / 8];
            unsigned char OperateNum = 1 << (7 - counter % 8);
            if ((OperateLUE & OperateNum) == 0) {
                CurrentFile_Address = sizeof(FileSystem_Struct_t) + counter * sizeof(File_Struct_t);
                FileSystem_Structure->FileLUT[counter / 8] |= OperateNum;
                break;
            }
        }
    } else {

    }
}

void FileSystem_CreateFiles(void) {
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BIN);
    CurrentFile_Structure = pvPortMalloc(sizeof(File_Struct_t));
    for (unsigned int counter = 0; counter < sizeof(File_Struct_t); counter++)
        ((unsigned char *) CurrentFile_Structure)[counter] = 0x00;
    CurrentFile_Structure->FileHead.Year = RTC_DateStruct.Year;
    CurrentFile_Structure->FileHead.Month = RTC_DateStruct.Month;
    CurrentFile_Structure->FileHead.Day = RTC_DateStruct.Date;
    CurrentFile_Structure->FileHead.Hour = RTC_TimeStruct.Hours;
    CurrentFile_Structure->FileHead.Minute = RTC_TimeStruct.Minutes;
    CurrentFile_Structure->FileHead.Second = RTC_TimeStruct.Seconds;
    CurrentFile_Structure->FileHead.EventLength = 1;
    CurrentFile_Structure->Event[0].Events = Poweron_Event;
    CurrentFile_Structure->Event[0].Hour = RTC_TimeStruct.Hours;
    CurrentFile_Structure->Event[0].Minute = RTC_TimeStruct.Minutes;
    CurrentFile_Structure->Event[0].Second = RTC_TimeStruct.Seconds;
    FileSystem_FindRemainSpace();
    if (FileSystem_Structure->FirstFileAddr == 0x00000) {
        FileSystem_Structure->FirstFileAddr = CurrentFile_Address;
        FileSystem_Structure->LastFileAddr = CurrentFile_Address;
    } else {
        LastFileHead_Structure = pvPortMalloc(sizeof(FileHead_Struct_t));
        LastFile_Address = FileSystem_Structure->LastFileAddr;
        W25QXX_Read((unsigned char *) LastFileHead_Structure, LastFile_Address, sizeof(FileHead_Struct_t));
        LastFileHead_Structure->NextFileAddr = CurrentFile_Address;
        FileSystem_Structure->LastFileAddr = CurrentFile_Address;
        CurrentFile_Structure->FileHead.NextFileAddr = 0x00000;
        CurrentFile_Structure->FileHead.LastFileAddr = LastFile_Address;
    }
    FileSystem_Structure->FileNum++;
}

void FileSystem_Config(void) {
    FileSystem_Structure = pvPortMalloc(sizeof(FileSystem_Struct_t));
    W25QXX_Read((unsigned char *) FileSystem_Structure, 0x00000, sizeof(FileSystem_Struct_t));
    if (FileSystem_Structure->FSHead != 0xA5) {
        GUI_Printf(19, 60, C_DARK_RED, C_WHITE, "Init FileSystem");
        FileSystem_FormatFlash();
    } else
        FileSystem_CreateFiles();
}
