#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  
	static char inited=0;

	if(!inited){
		lm_init();
		inited=1;
	}

	mmap_t map; // a variable that maps the physical addr, the virtual addr, and the size of the alloc'ed memory.

	lm_alloc(sizeof(vbe_mode_info_t), &map);

	phys_bytes buf=map.phys;
	struct reg86u r;

	r.u.w.ax = 0x4F01; /* VBE get mode info */
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf); /* set a segment base */
	r.u.w.di = PB2OFF(buf); /* set the offset accordingly */
	r.u.w.cx = mode;
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) { /* call BIOS */
		fprintf(stderr, "Couldn't call sys_int86!\n");
		return -5;
	}

	memcpy(vmi_p, map.virtual, map.size); // make a copy of this before freeing lm.
	lm_free(&map);
	return 0;
}


