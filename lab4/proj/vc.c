#include <lcom/lcf.h>
#include "vc.h"
#include <math.h>

unsigned int bppixel;
unsigned int x_res;
unsigned y_res;
unsigned color_mode;
unsigned int bytes_per_pixel;
static unsigned int vram_base;  
static unsigned int vram_size; 
vbe_mode_info_t mode_info;
struct reg86 r;  
static void* video_mem;
//static char* aux_buffer;

int (vbe_get_mode_inf)(uint16_t mode,vbe_mode_info_t *vmi_p){
  uint32_t size=sizeof(vbe_mode_info_t);
  mmap_t adr;
  lm_alloc(size,&adr); //liblm.h
  struct reg86 r;  
  memset(&r,0,sizeof(r)); 
  r.es = PB2BASE(adr.phys); //physical address //liblm.h
  r.di = PB2OFF(adr.phys);   //liblm.h     
  r.ax = 0x4F01;          
  r.cx = mode;
  r.intno = 0x10;
  *vmi_p=*(vbe_mode_info_t*)adr.virt;
  lm_free(&adr); //liblm.h
  return 0;
}

int (set_vbe_mode)(uint16_t mode){
  struct minix_mem_range mr;
  mr.mr_base=0; //lowest memory address in range
  mr.mr_limit=1<<20; //highest memory address in range
  memset(&r,0,sizeof(r));
  r.ax = 0x4f02;
  r.bx = BIT(14)|mode;
  r.intno = 0x10; //interrupt number

  if( sys_int86(&r) ==1){
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int (get_mode_info)(uint16_t mode){
  int s;
  vbe_get_mode_inf(mode, &mode_info);
  struct minix_mem_range mr;
  x_res=mode_info.XResolution;
  y_res=mode_info.YResolution;
  bppixel=mode_info.BitsPerPixel;
  bytes_per_pixel=(bppixel+7)/8;
  vram_base=(phys_bytes)mode_info.PhysBasePtr;
  vram_size=(x_res*y_res*bytes_per_pixel);

  /*Allow memory mapping*/
  mr.mr_base=vram_base;
  mr.mr_limit=mr.mr_base+vram_size;

  if( OK != (s = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    panic("sys_privctl (ADD_MEM) failed: %d\n", s);}

  /*Map memory*/
  video_mem=vm_map_phys(SELF,(void*)mr.mr_base,vram_size);
  if(video_mem == MAP_FAILED){
    panic("couldn’t map video memory");}
 // color_mode=mode_info.MemoryModel;
  //aux_buffer=(char*)malloc(vram_size);
  return 0; 
}

int (vg_draw_rectangle)(uint16_t	x, uint16_t  y, uint16_t width, uint16_t height, uint32_t color)
{

  for (uint16_t i = 0; i < height; i++) {
    vg_draw_hline(x, y + i, width, color);
  }

  return 0;
}


int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  if ( y >= y_res || x >= x_res){
    return 1;
  }
  
    uint8_t bytes_per_pixel =  ceil(bppixel / 8.0);

    if (bytes_per_pixel == 1) *((char*)video_mem + (y*x_res*bytes_per_pixel)+(x*bytes_per_pixel) ) = (unsigned char) color;
    else
    {
      for(int i = 0; i < bytes_per_pixel; i++)
      {
        *((char*)video_mem + (bytes_per_pixel*y*x_res) + (x*bytes_per_pixel) + i) = (uint8_t)color;
        color=color>>8;
      }
      
    }
        
    return 0;
}

int (vg_draw_hline)(uint16_t	x, uint16_t  y, uint16_t len, uint32_t color)
{
 for (uint16_t j = 0; j < len; j++) {
   draw_pixel((uint16_t)x +j, y, color);
 }
  return 0; 
}

int draw_xpm(int x ,int y, xpm_map_t xpm){
  uint8_t* sprite; 
  xpm_image_t img;
  sprite=xpm_load(xpm,XPM_INDEXED, &img);
  //memset(v_mem,0xff,x_res*y_res*2);
  int bcount=0;
  
  for (int i=y; i< img.height+y;i++){
    for(int j=x;j<img.width+x;j++){
      if(((uint16_t*)(img.bytes))[bcount]!=xpm_transparency_color(img.type))
        draw_pixel(j,i,((uint16_t*)(img.bytes))[bcount]);
      bcount++;
    }
  }

 return 0;
}
/*
int draw_sprite(int x ,int y, xpm_image_t *img){
  int bcount=0;  
  for (int i=y; i< img->height+y;i++){
    for(int j=x;j<img->width+x;j++){
      if(((uint16_t*)(img->bytes))[bcount]!=xpm_transparency_color(img->type))
        draw_pixel(j,i,((uint16_t*)(img->bytes))[bcount]);
      bcount++;
    }
  }

 return 0;
}

int draw_background(int x,int y,int speed, xpm_image_t sprite){

  xpm_image_t img = sprite;
  uint8_t *temp=(img.bytes);
  const int img_pixel = img.width* bytes_per_pixel;
  x=(x)%img.width;
  temp+=x*bytes_per_pixel;
  for(int row=0;row<sprite.height;++row){
    memcpy(aux_buffer+row*img_pixel,temp+row*img_pixel,img_pixel);
    
  }
  return 0;
}

int refresh_graphics(){
  memcpy(video_mem, aux_buffer , x_res * y_res * (bppixel/8) );
  return 0;
}

*/
