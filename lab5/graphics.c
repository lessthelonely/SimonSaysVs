#include <lcom/lcf.h>
#include <machine/int86.h> // /usr/src/include/arch/i386

#include "graphics.h"
#include "vbe.h"
#include <math.h>

static char *video_mem;         /* Process (virtual) address to which VRAM is mapped */
static unsigned h_res;          /* Horizontal resolution in pixels */
static unsigned v_res;          /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel;
static uint8_t RedMaskSize;
static uint8_t BlueMaskSize;
static uint8_t GreenMaskSize;
static uint8_t RedFieldPosition;
static uint8_t BlueFieldPosition;
static uint8_t GreenFieldPosition;

static int max_sprites;
static int num_sprites;
static sprite_mov_t *sprites;

static char* aux_buffer;

int graphics_set_mode(uint16_t mode)
{

  /* Specify the appropriate register values */
  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); /* zero the structure */

  r86.intno = INTNO;
  r86.ah = VBE_CALL_REG_VAL;
  r86.al = VBE_SET_MODE;
  r86.bx = SET_LINEAR | mode;

  /* Make the BIOS call */

  if (sys_int86(&r86) != OK)
  {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int mode_get_info(uint16_t mode)
{
  vbe_mode_info_t mode_info;
  vbe_get_mode_info(mode, &mode_info);
  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = ceil(bits_per_pixel+7 / 8.0);
  //bytes_per_pixel = ceil(bits_per_pixel / 8.0);
  RedMaskSize = mode_info.RedMaskSize;
  BlueMaskSize = mode_info.BlueMaskSize;
  GreenMaskSize = mode_info.GreenMaskSize;
  RedFieldPosition = mode_info.RedFieldPosition;
  BlueFieldPosition = mode_info.BlueFieldPosition;
  GreenFieldPosition = mode_info.GreenFieldPosition;
  map_video_mem(mode_info.PhysBasePtr, h_res * v_res * bytes_per_pixel);
  return 0;
}

int draw_sprite(int id)
{
  uint32_t color;
  int byte_count = 0;
  int y = (int)sprites[id].yi;
  int x = (int)sprites[id].xi;
  xpm_image_t *img = &(sprites[id].img);
  for (int i = y; i < (*img).height + y; i++)
  {
    for (int j = x; j < (*img).width + x; j++)
    {
      color = 0;
      for (unsigned int k = 0; k < bytes_per_pixel; k++)
      {
        color = color << 8;
        color |= (*img).bytes[byte_count];
        byte_count++;
      }
      draw_pixel(j, i, color);
    }
  }
  printf("Drew sprite\n");
  return 0;
}

int erase_sprite(int id)
{
  uint32_t color = xpm_transparency_color(XPM_INDEXED); //TODO change according to grpahics type
  int y = (int)sprites[id].yi;
  int x = (int)sprites[id].xi;
  xpm_image_t *img = &(sprites[id].img);
  for (int i = y; i < (*img).height + y; i++)
  {
    for (int j = x; j < (*img).width + x; j++)
    {
      draw_pixel(j, i, color);
    }
  }
  printf("Drew sprite\n");
  return 0;
}

int move_sprite(int id)
{
  sprites[id].xi += sprites[id].speed_x;
  if (sprites[id].xi > (float)sprites[id].xf)
  {
    sprites[id].xi = sprites[id].xf;
    sprites[id].speed_x = 0;
  }

  sprites[id].yi += sprites[id].speed_y;
  if (sprites[id].yi > (float)sprites[id].yf)
  {
    sprites[id].yi = sprites[id].yf;
    sprites[id].speed_y = 0;
  }
  return 0;
}

int draw_pix_map(xpm_map_t xpm, uint16_t x, uint16_t y)
{
  xpm_image_t img;
  uint8_t *map;
  uint32_t color;
  int byte_count = 0;
  // get the pixmap from the XPM
  map=xpm_load(xpm, XPM_5_6_5, &img);
  for (int i = y; i < img.height + y; i++)
  {
    for (int j = x; j < img.width + x; j++)
    {
      color = 0;
      for (unsigned int k = 0; k < bytes_per_pixel; k++)
      {
        color = color << 8;
        color |= img.bytes[byte_count];
        if(((uint16_t*)(img.bytes))[byte_count]!=xpm_transparency_color(img.type))
        draw_pixel(j,i,((uint16_t*)(img.bytes))[byte_count]);
      
        byte_count++;
      }
      //draw_pixel(j, i, color);
    }
  }
  return 0;
}

int draw_rectangle(uint16_t x, uint16_t y,
                   uint16_t width, uint16_t height, uint32_t color)
{
  for (int i = y; i < y + height; i++)
  {
    vg_draw_hline(x, i, width, color);
  }
  return 0;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color)
{
  char *left = video_mem + (y * h_res + x) * bytes_per_pixel;
  for (unsigned int j = 0; j < bytes_per_pixel; j++)
  {
    *left = (char)(color >> (8 * j));
    left++;
  }
  return 0;
}

void create_sprite_memory(int max_num_sprites)
{
  sprites = malloc(sizeof(sprite_mov_t) * max_num_sprites);
  max_sprites = max_num_sprites;
}

int add_sprite(xpm_map_t xpm, float xi, float yi, uint16_t xf, uint16_t yf, float speed_x, float speed_y)
{
  /*
  xpm_image_t img;
  uint8_t *map;
  uint32_t color;
  int byte_count = 0;
  // get the pixmap from the XPM
  map = xpm_load(xpm, XPM_INDEXED, &img);
  */
  printf("Adding sprite\n");
  xpm_load(xpm, XPM_INDEXED, &sprites[num_sprites].img);
  printf("Sprite loaded");

  sprites[num_sprites].id = num_sprites;
  sprites[num_sprites].xi = xi;
  sprites[num_sprites].yi = yi;
  sprites[num_sprites].xf = xf;
  sprites[num_sprites].yf = yf;
  sprites[num_sprites].speed_x = speed_x;
  sprites[num_sprites].speed_y = speed_y;
  //sprites[num_sprites].img; //already set in the xpm_load
  num_sprites++;
  printf("Sprite added\n");
  return num_sprites - 1;
}

void remove_sprite_memory()
{
  free(sprites);
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
  char *left = video_mem + (y * h_res + x) * bytes_per_pixel;
  for (int i = 0; i < len; i++)
  {
    for (unsigned int j = 0; j < bytes_per_pixel; j++)
    {
      *left = (char)(color >> (8 * j));
      left++;
    }
  }
  return 0;
}
int map_video_mem(unsigned int vram_base, unsigned int vram_size)
{
  struct minix_mem_range mr;
  int r;

  mr.mr_base = (phys_bytes)vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  aux_buffer = (char*) malloc(vram_size);

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
  {
    panic("couldn't map video memory");
  }
  return 0;
}

void get_rectangles_size(uint8_t no_rectangles, uint8_t *width, uint8_t *height)
{
  *width = h_res / no_rectangles;
  *height = v_res / no_rectangles;
  printf("%d, %d", *width, *height);
}

uint32_t get_rectangles_color(uint16_t mode, uint32_t first, uint8_t no_rectangles, uint8_t step, uint8_t row, uint8_t col)
{
  if (mode == 0x105)
  {
    return (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
  }
  uint8_t red = ((first >> RedFieldPosition) & ((1 << RedMaskSize) - 1) + col * step) % (1 << RedMaskSize);
  uint8_t green = ((first >> GreenFieldPosition) & ((1 << GreenMaskSize) - 1) + row * step) % (1 << GreenMaskSize);
  uint8_t blue = ((first >> BlueFieldPosition) & ((1 << BlueMaskSize) - 1) + (col + row) * step) % (1 << BlueMaskSize);
  return red | green << GreenFieldPosition | blue << BlueFieldPosition;
}
