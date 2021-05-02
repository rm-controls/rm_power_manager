//
// Created by Lao·Zhu on 2021/5/3.
//

#include "fs_operate.h"
#include "ff.h"
#include "FreeRTOS.h"
#include "string.h"
#include "stdio.h"

FATFS *fs;

void FileSystem_ScanFiles(unsigned char **filename) {
    FRESULT res;
    DIR dir;
    FILINFO tmp_fileinfo;
    unsigned char file_counter = 0;
    res = f_opendir(&dir, "0:");
    if (res == FR_OK) {
        file_counter++;
        while (1) {
            res = f_readdir(&dir, &tmp_fileinfo);
            if (res != FR_OK || tmp_fileinfo.fname[0] == 0)
                break;
            sprintf((char *) &filename[0][file_counter], "0:\\%s", tmp_fileinfo.fname);
        }
    }
}

void FileSystem_GetFree(unsigned int *total, unsigned int *free) {
    FATFS *tmp_fs;
    unsigned char res;
    unsigned int fre_clust = 0, fre_sect, tot_sect;
    res = (unsigned int) f_getfree("0:", (DWORD *) &fre_clust, &tmp_fs);
    if (res == 0) {
        tot_sect = (tmp_fs->n_fatent - 2) * tmp_fs->csize;
        fre_sect = fre_clust * tmp_fs->csize;
        *total = tot_sect >> 1;
        *free = fre_sect >> 1;
    }
}

void FileSystem_Config(void) {
    unsigned char result;
    fs = pvPortMalloc(sizeof(FATFS));
    result = f_mount(fs, "0:", 1);
    if (result == FR_NO_FILESYSTEM) {
        MKFS_PARM fs_param;
        memset(&fs_param, 0x00, sizeof(MKFS_PARM));
        fs_param.fmt = FM_FAT32;
        f_mkfs("0:", &fs_param, NULL, 512 * 8);  // 8个扇区为1簇
    }
}
