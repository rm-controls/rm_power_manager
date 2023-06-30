//
// Created by LaoZhu on 2023/6/30.
//

#ifndef POWER_MANAGER_RMFS_FILESYSTEM_H_
#define POWER_MANAGER_RMFS_FILESYSTEM_H_

void filesystem_config(void);
unsigned short get_rmfs_remain(void);
unsigned char get_rmfs_status(void);
void make_rmfs_table(void);

#endif  // POWER_MANAGER_RMFS_FILESYSTEM_H_
