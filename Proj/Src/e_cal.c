#include <stdio.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <machine/int86.h>
#include <sys/time.h>
#include <time.h>

#include "vbe.h"
#include "i8042.h"
#include "i8254.h"
#include <minix/sysutil.h>
#include "lmlib.h"

#include "keyboard.h"
#include "timer.h"
#include "video.h"
#include "mouse.h"

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

void *init() {
	video_m=vg_init(0x118);
	video_info_t vi;
	vi = get_vi();
	int i, j;

	fill_screen(white_c);

	for(i = 0;i<50;i++){
		for(j=0;j<50;j++){
			setP(i,j,blue_c);
		}
	}

	sleep(5);

	vg_exit();

	return 0;
}
