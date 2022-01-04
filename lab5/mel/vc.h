#ifndef _LCOM_VC_H_
#define _LCOM_VC_H_

#include <lcom/lcf.h>

#define INT 0x10
#define VBE 0x02
#define VBE_CNT 0x00
#define VBE_INFO 0x01

int vbe_get_mode_inf(uint16_t mode, vbe_mode_info_t *vmi_p);
int set_vbe_mode(uint16_t mode);	
int get_mode_info(uint16_t mode);
int (vg_draw_rectangle)(uint16_t	x, uint16_t  y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_hline)(uint16_t	x, uint16_t  y, uint16_t len, uint32_t color);
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);
int draw_back(int x,int y, xpm_image_t xpm);

#endif

