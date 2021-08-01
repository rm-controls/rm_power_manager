//
// Created by Lao·Zhu on 2021/2/2.
//

#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

typedef struct {
    unsigned char PID;
    unsigned char Data[8];
} DTP_Package_t;

extern DTP_Package_t Upload_Package;
extern float Parameters[4];
void DTP_Transmit(DTP_Package_t *dtp_package);

#endif
