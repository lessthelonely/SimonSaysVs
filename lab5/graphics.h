#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <lcom/lcf.h>


#define TRANSPARENCY_COLOR_1_5_5_5 0x8000
#define TRANSPARENCY_COLOR_8_8_8_8 0xFF000000
#define CHROMA_KEY_GREEN_888 0x00b140
#define CHROMA_KEY_GREEN_565 0x0588
/*
enum xpm_image_type
{
    XPM_INDEXED,
    XPM_1_5_5_5,
    XPM_5_6_5,
    XPM_8_8_8,
    XPM_8_8_8_8,
    INVALID_XPM
};
typedef struct
{
    enum xpm_image_type type;
    uint16_t width;
    uint16_t height;
    size_t size;
    uint8_t *bytes;
} xpm_image_t;
*/

typedef struct{
    unsigned int id;
    float xi, yi;
    uint16_t xf, yf;
    float speed_x, speed_y;
    xpm_image_t img;

}sprite_mov_t;

int graphics_set_mode(uint16_t mode);

int mode_get_info(uint16_t mode);

int draw_sprite(int id);

int erase_sprite(int id);

int move_sprite(int id);

int draw_pix_map(xpm_map_t xpm, uint16_t xi, uint16_t yi);

int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int draw_pixel(uint16_t x, uint16_t y, uint32_t color);

void create_sprite_memory(int max_num_sprites);

int add_sprite(xpm_map_t xpm, float xi, float yi, uint16_t xf, uint16_t yf, float speed_x, float speed_y);

void remove_sprite_memory();

int map_video_mem(unsigned int vram_base, unsigned int vram_size);
// int vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color); //declared by the program

void get_rectangles_size(uint8_t no_rectangles, uint8_t *width, uint8_t *height);

uint32_t get_rectangles_color(uint16_t mode, uint32_t first, uint8_t no_rectangles, uint8_t step, uint8_t row, uint8_t col);

#endif //GRAPHICS_H
