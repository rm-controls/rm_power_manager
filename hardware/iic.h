//
// Created by LaoZhu on 2023/6/6.
//

#ifndef POWER_MANAGER_HARDWARE_IIC_H_
#define POWER_MANAGER_HARDWARE_IIC_H_

void iic_config(void);

void iic_start(void);
void iic_stop(void);
void iic_send_ack(void);
void iic_send_nack(void);
unsigned char iic_wait_ack(void);
void iic_send_byte(unsigned char data);
unsigned char iic_read_byte(void);

#endif //POWER_MANAGER_HARDWARE_IIC_H_
