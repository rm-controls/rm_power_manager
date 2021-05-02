#include "ff.h"
#include "diskio.h"
#include "flash.h"

#define FLASH_SECTOR_SIZE    512    // 每个扇区为512字节
#define FLASH_SECTOR_COUNT   1024   // 512 * 1024 = 512KB
#define FLASH_BLOCK_SIZE     8      // 每个BLOCK有8个扇区

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
        W25QXX_Read(buff, sector * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
        sector++;
        buff += FLASH_SECTOR_SIZE;
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
        W25QXX_Write((unsigned char *) buff, sector * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
        sector++;
        buff += FLASH_SECTOR_SIZE;
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
        case GET_SECTOR_SIZE:*(WORD *) buff = FLASH_SECTOR_SIZE;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:*(WORD *) buff = FLASH_BLOCK_SIZE;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:*(DWORD *) buff = FLASH_SECTOR_COUNT;
            res = RES_OK;
            break;
        default:res = RES_PARERR;
            break;
    }
    return res;
}

