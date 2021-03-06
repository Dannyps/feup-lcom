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
#include "utils.h"
#include "view.h"
#include "utils.h"

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

extern xpm_t XPM_landingPage;

void *init(char* argv0) {
	video_m=vg_init(0x118);
	srand(time(NULL)); // randomize seed
	rfill_screen();
	load_xpms(argv0);
	draw_main_page();
	start_listening();

	vg_exit();

	return 0;
}
