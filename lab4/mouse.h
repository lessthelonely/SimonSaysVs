#ifndef _LCOM_mouse_H_
#define _LCOM_mouse_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>

int sys_inb_cnt(port_t port, uint8_t *value);

int mouse_subscribe_int(uint8_t *bit_no);

int mouse_unsubscribe_int();

int kbc_read_status(uint8_t *st);

int kbc_read_buf(uint8_t *sc_byte);

bool kbc_check_status();

void mouse_parse_packet(struct packet *pp,uint8_t bytes[],uint8_t size);

int kbc_command_handler(uint32_t cmd,uint32_t arg);

bool kbc_check_out_buf ();

bool kbc_check_in_buf();

int kbc_write_command(uint32_t cmd);

int kbc_write_argument(uint32_t arg);

int mouse_write_command(uint32_t cmd, uint8_t *resp);

extern bool gesture;

enum state {init, drag1, wait, drag2};

struct mouse_ev* mouse_get_event(struct packet *pp);

void mouse_detect_gesture(struct mouse_ev *evt, uint8_t x_len, uint8_t tolerance);


#endif /* _LCOM_mouse_H */
