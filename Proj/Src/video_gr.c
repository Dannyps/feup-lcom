#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "video_gr.h"
#include "vbe.h"


/* Private global variables */

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static video_info_t vi;		/* Horizontal screen resolution in pixels */

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

void* vg_init(unsigned short mode) {

	vbe_mode_info_t vmi;
	if(vbe_get_mode_info(0x105, &vmi)!=0){
		printf("Couldn't execute vbe function 01h. Exiting.\n");
		exit(4);
	}


	/* Store retrieved info for later usage */
	vi.y=vmi.YResolution;
	vi.x=vmi.XResolution;
	vi.bpp=vmi.BitsPerPixel;


	// effectively start graphics mode.
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg86.u.w.bx = 1<<14|mode;

	if( sys_int86(&reg86) != OK ) {
	  printf("\tvg_exit(): sys_int86() failed \n");
	  return NULL;
	}
	int r;
	struct mem_range mr;
	unsigned int vram_base=vmi.PhysBasePtr; /* VRAM physical address */
	unsigned int vram_size=vmi.XResolution*vmi.YResolution*vmi.BitsPerPixel; /* VRAM size */


	/* Allow memory mapping */
	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
	if(video_mem == MAP_FAILED)
		panic("couldn't map video memory");

	return video_mem;
}

video_info_t get_vi(){
	printf("get vi here: %d\n", vi.x);
	return vi;
}

