#include "ff.h"
#include "diskio.h"
#include "flash.h"
#include "rtc.h"

#define NORFLASH_SECTOR_SIZE    512    // 每个扇区为512字节
#define NORFLASH_SECTOR_COUNT   1024   // 512 * 1024 = 512KB
#define NORFLASH_BLOCK_SIZE     8      // 每个BLOCK有8个扇区

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status(
    BYTE pdrv
) {
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize(
    BYTE pdrv
) {
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read(
    BYTE pdrv,
    BYTE *buff,
    LBA_t sector,
    UINT count
) {
    for (; count > 0; count--) {
        W25QXX_Read(buff, sector * NORFLASH_SECTOR_SIZE, NORFLASH_SECTOR_SIZE);
        sector++;
        buff += NORFLASH_SECTOR_SIZE;
    }
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if FF_FS_READONLY == 0
DRESULT disk_write(
    BYTE pdrv,
    const BYTE *buff,
    LBA_t sector,
    UINT count
) {
    for (; count > 0; count--) {
        W25QXX_Write((unsigned char *) buff, sector * NORFLASH_SECTOR_SIZE, NORFLASH_SECTOR_SIZE);
        sector++;
        buff += NORFLASH_SECTOR_SIZE;
    }
    return RES_OK;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl(
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE cmd,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
) {
    DRESULT res;
    switch (cmd) {
        case CTRL_SYNC:res = RES_OK;
            break;
        case GET_SECTOR_SIZE:*(WORD *) buff = NORFLASH_SECTOR_SIZE;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:*(WORD *) buff = NORFLASH_BLOCK_SIZE;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:*(DWORD *) buff = NORFLASH_SECTOR_COUNT;
            res = RES_OK;
            break;
        default:res = RES_PARERR;
            break;
    }
    return res;
}

DWORD get_fattime(void) {
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BIN);
    return ((2000UL - 1980 + RTC_DateStruct.Year) << 25) /* Year = 2010 */
        | (RTC_DateStruct.Month << 21) /* Month = 11 */
        | (RTC_DateStruct.Date << 16) /* Day = 2 */
        | (RTC_TimeStruct.Hours << 11) /* Hour = 15 */
        | (RTC_TimeStruct.Minutes << 5) /* Min = 0 */
        | (RTC_TimeStruct.Seconds >> 1) /* Sec = 0 */
        ;
}
