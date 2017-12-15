#include <sys/time.h>
#include <time.h>
#include "vbe.h"
#include "video_gr.h"
#include "i8042.h"
#include "i8254.h"
#include <minix/sysutil.h>
#include "lmlib.h"
#include "month_pixmap.h"
#include "keyboard.h"
#include "timer.h"
#include "video.h"
#include "mouse.h"
#include "view.h"

void nextMonth(View *v) {
	if(v->month != 12) v->month += 1;
	else {
		v->month = 1;
		v->year += 1;
	}
}

void prevMonth(View *v) {
	if(v->month != 1) v->month -= 1;
	else {
		v->month = 12;
		v->year -= 1;
	}
}

void drawMonth(View v, int x, int y) {
	switch(v.month) {
	case 1: draw_xpm(January, x, y); break;
	case 2: draw_xpm(February, x, y); break;
	case 3: draw_xpm(March, x, y); break;
	case 4: draw_xpm(April, x, y); break;
	case 5: draw_xpm(May, x, y); break;
	/*case 6: draw_xpm(June, x, y); break;
	case 7: draw_xpm(July, x, y); break;
	case 8: draw_xpm(August, x, y); break;
	case 9: draw_xpm(September, x, y); break;
	case 10: draw_xpm(October, x, y); break;
	case 11: draw_xpm(November, x, y); break;
	case 12: draw_xpm(December, x, y); break;*/
	}
}
