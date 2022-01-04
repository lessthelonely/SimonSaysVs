#ifndef _LCOM_kbc_H_
#define _LCOM_kbc_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>

#ifdef LAB3
int sys_inb_cnt(port_t port, uint8_t *value);
#else
#  define sys_inb_cnt(p, q) sys_inb(p, q)
#endif

int kbc_subscribe_int(uint8_t *bit_no);

int kbc_unsubscribe_int();

int kbc_read_status(uint8_t *st);

int kbc_read_buf(uint8_t *sc_byte);

bool kbc_check_status();

int kbc_wrt_cmd (uint32_t cmd,uint32_t args);


#endif /* _LCOM_kbc_H */
