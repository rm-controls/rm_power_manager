//
// Created by Lao·Zhu on 2021/5/3.
//

#ifndef PROGRAM_DEVICES_FS_OPERATE_H_
#define PROGRAM_DEVICES_FS_OPERATE_H_

void FileSystem_Config(void);
void FileSystem_FormatFlash(void);
void FileSystem_CreateFiles(const char *Filename);
void FileSystem_GetFree(unsigned int *total, unsigned int *free);
void FileSystem_ScanFiles(unsigned char **filename);

#endif //PROGRAM_DEVICES_FS_OPERATE_H_
