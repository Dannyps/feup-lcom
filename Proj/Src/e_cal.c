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

#include "view.h"
#include "month_pixmap.h"

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

void *init() {
	video_m=vg_init(0x118);
	video_info_t vi;
	vi = get_vi();
	int i, j;

	rfill_screen();
	printf("%d %d %d\n", white_c.r, white_c.g, white_c.b);
	printf("filled screen\n");;
	for(i = 0;i<vi.x;i++){
		for(j=0;j<100;j++){
			setP(i,j,white_c);
		}
	}

	View cal = {2017, 1, 2};

	draw_xpm(lcom_nome, 0, 0);
	draw_xpm(lcom_tcti, 724, 0);

	//sleep(5);
    printf("waiting for esc");
    // ESC key scan
    kbd_test_scan();
	vg_exit();

	return 0;
}
