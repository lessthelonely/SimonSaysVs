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
static char*aux_buffer;

int (vbe_get_mode_inf)(uint16_t mode,vbe_mode_info_t *vmi_p){
  uint32_t size=sizeof(vbe_mode_info_t);   
  mmap_t adr;
  lm_alloc(size,&adr); 
  struct reg86 r;  
  memset(&r,0,sizeof(r)); 
  r.es = PB2BASE(adr.phys); //physical address //liblm.h
  r.di = PB2OFF(adr.phys);   //Sets offset accordingly (liblm.h)     
  r.ax = 0x4F01; //VBE get mode info       
  r.cx = mode;
  r.intno = 0x10;
  //*vmi_p=*(vbe_mode_info_t*)adr.virt;
  memcpy(vmi_p, adr.virt, sizeof(vbe_mode_info_t));
  lm_free(&adr); //Free the allocated memory (liblm.h)
  return 0;
}

int (set_vbe_mode)(uint16_t mode){
  memset(&r,0,sizeof(r)); //Zero the struct
  r.ax = 0x4f02; //VBE call, function 02, sets the VBE mode
  r.bx = BIT(14)|mode; //BIT(14)<=>1<<14 
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
  aux_buffer = (char*) malloc(vram_size);
  
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
   draw_pixel(x +j, y, color);
 }
  return 0; 
}

int (draw_xpm)(int x ,int y, xpm_image_t xpm){
  //uint8_t* sprite; 
  //xpm_image_t img;
  //sprite=xpm_load(xpm,XPM_5_6_5,&img);
  //memset(v_mem,0xff,x_res*y_res*2);
  int bcount=0;
  
  for (int i=y; i< xpm.height+y;i++){
    for(int j=x;j<xpm.width+x;j++){
      if(((uint16_t*)(xpm.bytes))[bcount]!=xpm_transparency_color(xpm.type))
        draw_pixel(j,i,((uint16_t*)(xpm.bytes))[bcount]);
      bcount++;
    }
  }

 return 0;
}

