//
// Created by LaoZhu on 2023/6/30.
//

#include "filesystem.h"
#include "w25qxx.h"

static unsigned short used_sector = 0;
static unsigned char filesystem_status = 0;

static struct log_table_element {
  unsigned int started_page;
  unsigned int page_length;
} *log_table;

void filesystem_config(void) {
    unsigned char flash_sector_table[2];
    unsigned short total_sector = w25qxx_get_total_sector();
    if (total_sector < 128)
        filesystem_status = 1;
    else {
        total_sector = total_sector - 4;
        w25qxx_read(flash_sector_table, 0x000000UL, 2);
        used_sector = ((flash_sector_table[0] << 8) | flash_sector_table[1]);
        if (used_sector > total_sector)
            filesystem_status = 2;
    }
}

unsigned short get_rmfs_remain(void) {
    if (filesystem_status == 0)
        return (w25qxx_get_total_sector() - used_sector - 4);
    return 0;
}

unsigned char get_rmfs_status(void) {
    return filesystem_status;
}

void make_rmfs_table(void) {
    unsigned char flash_sector_table[2] = {0};
    w25qxx_erase_chip();
    w25qxx_write(flash_sector_table, 0x000000UL, 2);
}

void get_rmfs_log_page(unsigned short index) {
    if (index <= used_sector) {
    
    }
}
