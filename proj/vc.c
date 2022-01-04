#include <lcom/lcf.h>
#include "vc.h"
#include "num_and_letters.h"
#include <math.h>

int vbe_get_mode_inf(unsigned short mode, vbe_mode_info_t *vmi_p)
{
    uint32_t size=sizeof(vbe_mode_info_t);
    mmap_t address;
    lm_alloc(size,&address); //allocate memory block
    memset(&r,0,sizeof(r));//Zero the struct 
    r.ax = 0x4F01; // VBE call, function 01 -- get VBE mode
    r.cx=mode;
    r.es=PB2BASE(address.phys); //Set a segment base
    r.di=PB2OFF(address.phys); //Set the offset accordingly
    r.intno=0x10;
    memcpy(vmi_p,address.virt,size);
    lm_free(&address); //free memory block
  return 0;
}

int(set_vbe_mode)(uint16_t mode)
{

  memset(&r, 0, sizeof(r));
  r.ah = 0x4f;
  r.al = VBE;
  r.bx = BIT(14) | mode;
  r.intno = INT; //interrupt number

  if (sys_int86(&r) == 1)
  {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int(get_mode_info)(uint16_t mode)
{
  int s;
  //vbe_get_mode_inf(mode, &mode_info);
  vbe_get_mode_info(mode, &mode_info);
  struct minix_mem_range mr;
  x_res = mode_info.XResolution;
  y_res = mode_info.YResolution;
  bppixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bppixel) / 8;
  vram_base = (phys_bytes)mode_info.PhysBasePtr;
  vram_size = (x_res * y_res * bytes_per_pixel);

  /*Allow memory mapping*/
  mr.mr_base = vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (s = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
  {
    panic("sys_privctl (ADD_MEM) failed: %d\n", s);
  }

  /*Map memory*/
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  buffer = malloc(sizeof(char) * x_res * y_res * bytes_per_pixel);
  if (video_mem == MAP_FAILED)
  {
    panic("Couldn’t map video memory");
  }

  return 0;
}

int get_mode_width()
{
  return WIDTH;
}

int get_mode_height()
{
  return HEIGHT;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color)
{
  if (y >= y_res || x >= x_res)

  {

    return 1;
  }

  uint8_t bytes_per_pixel = ceil(bppixel / 8.0);

  if (bytes_per_pixel == 1)

    *((char *)buffer + (y * x_res * bytes_per_pixel) + (x * bytes_per_pixel)) = (unsigned char)color;

  else

  {

    for (int i = 0; i < bytes_per_pixel; i++)

    {

      *((char *)buffer + (bytes_per_pixel * y * x_res) + (x * bytes_per_pixel) + i) = (uint8_t)color;

      color = color >> 8;
    }
  }
  return 0;
}

void copy_to_video()
{
  memcpy(video_mem, buffer, x_res * y_res * bytes_per_pixel);
  memset(buffer, 0, x_res * y_res * bytes_per_pixel);
}

int(draw_xpm)(xpm_image_t screen)
{
  memcpy(buffer, screen.bytes, x_res * y_res * bytes_per_pixel);
  return 0;
}

int draw_xpm_x_y(xpm_image_t img, uint16_t x, uint16_t y)
{
  int bcount = 0;

  for (int i = y; i < img.height + y; i++)
  {
    for (int j = x; j < img.width + x; j++)
    {
      if (((uint16_t *)(img.bytes))[bcount] != xpm_transparency_color(img.type))
        draw_pixel(j, i, ((uint16_t *)(img.bytes))[bcount]);
      bcount++;
    }
  }

  return 0;
}

struct text_box create_writeable_text_box(uint16_t cursor_x, uint16_t cursor_y, uint16_t min_x, uint16_t max_x, int max_size)
{
  struct text_box box;
  box.cursor_x = cursor_x;
  box.xi = cursor_x;
  box.yi = cursor_y;
  box.max_x = max_x;
  box.min_x = min_x;
  box.char_max_size = max_size;
  box.char_size = 0;
  return box;
}

struct text_box create_static_text_box(uint16_t x, uint16_t y, char char_props[], int size)
{
  struct text_box box = create_writeable_text_box(x, y, 0, UINT16_MAX, size);
  for (int i = 0; i < size; i++)
  {
    add_to_text_box(char_props[i], &box);
  }
  return box;
}

void draw_text_box(struct text_box *box)
{
  int x = (*box).xi;
  int y = (*box).yi;
  for (int i = 0; i < (*box).char_size; i++)
  {
    draw_xpm_x_y(*(*box).char_prop[i], x, y);
    x += (*(*box).char_prop[i]).width;
  }
}

void erase_text_box(struct text_box *box)
{
  int x = (*box).xi;
  int y = (*box).yi;
  xpm_image_t empty = get_empty();
  for (int i = 0; i < (*box).char_size; i++)
  {
    draw_xpm_x_y(empty, x, y);
    x += empty.width;
  }
}

bool add_to_text_box(char byte, struct text_box *box)
{
  if ((*box).char_size == (*box).char_max_size)
  {
    return false;
  }
  xpm_image_t *img = get_prop_from_char(byte);
  if ((*box).cursor_x + (*img).width > (*box).max_x)
  {
    return false;
  }
  (*box).char_prop[(*box).char_size] = img;
  (*box).char_size++;
  (*box).cursor_x += (*img).width;
  return true;
}

bool add_to_centered_text_box(char byte, struct text_box *box)
{
  if ((*box).char_size == (*box).char_max_size)
  {
    return false;
  }
  xpm_image_t *img = get_prop_from_char(byte);
  if ((*box).cursor_x + (*img).width / 2 > (*box).max_x)
  {
    return false;
  }
  if ((*box).xi - ((*img).width + 1 / 2) < (*box).min_x)
  {
    return false;
  }
  (*box).char_prop[(*box).char_size] = img;
  (*box).char_size++;
  (*box).cursor_x += (*img).width / 2;
  (*box).xi -= ((*img).width + 1) / 2;
  return true;
}

void remove_from_centered_text_box(struct text_box *box)
{
  if ((*box).char_size == 0)
  {
    return;
  }
  erase_text_box(box);
  xpm_image_t *img = (*box).char_prop[(*box).char_size - 1];
  (*box).char_size--;
  (*box).cursor_x -= (*img).width / 2;
  (*box).xi += ((*img).width + 1) / 2;
}
