//
// Created by Lao·Zhu on 2021/5/3.
//

#include "gui_base.h"
#include "fs_operate.h"
#include "FreeRTOS.h"
#include "stdio.h"

void FileSystem_FormatFlash(void) {

}

void FileSystem_CreateFiles(const char *Filename) {

}

void FileSystem_Config(void) {
    GUI_Printf(19, 60, C_DARK_RED, C_WHITE, "Init FileSystem");
    FileSystem_FormatFlash();
}
