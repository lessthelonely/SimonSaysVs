#include <machine/int86.h>

int sys_int86(struct reg86 *reg86p); 
<machine/int86.h>.

/* Set default text mode */
int vg_exit() {
  struct reg86 r86;
   
  /* Specify the appropriate register values */
  
  memset(&r86, 0, sizeof(r86));	/* zero the structure */

  r86.intno = 0x10; /* BIOS video services */
  r86.ah = 0x00;    /* Set Video Mode function */
  r86.al = 0x03;    /* 80x25 text mode*/
  
  /* Make the BIOS call */

  if( sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

void *vm_map_phys(endpoint_t who, void *phaddr, size_t len);
int sys_privctl(endpoint_t proc_ep, int request, void *p);

#include <minix/driver.h>
#include <sys/mman.h>

[...]

static void *video_mem;         /* frame-buffer VM address 
				   (static global variable*/ 
[...]

struct minix_mem_range mr;
unsigned int vram_base;  /* VRAM's physical addresss */
unsigned int vram_size;  /* VRAM's size, but you can use
				    the frame-buffer size, instead */
int r;				    

/* Use VBE function 0x01 to initialize vram_base and vram_size */

[...]

/* Allow memory mapping */

mr.mr_base = (phys_bytes) vram_base;	
mr.mr_limit = mr.mr_base + vram_size;  

if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
   panic("sys_privctl (ADD_MEM) failed: %d\n", r);

/* Map memory */

video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

if(video_mem == MAP_FAILED)
   panic("couldn't map video memory");