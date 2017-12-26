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

void drawMonthName(View v, int x, int y) {
	switch(v.month) {
	case 1: draw_xpm(January, x, y); break;
	case 2: draw_xpm(February, x, y); break;
	case 3: draw_xpm(March, x, y); break;
	/*case 4: draw_xpm(April, x, y); break;
	case 5: draw_xpm(May, x, y); break;
	case 6: draw_xpm(June, x, y); break;
	case 7: draw_xpm(July, x, y); break;
	case 8: draw_xpm(August, x, y); break;
	case 9: draw_xpm(September, x, y); break;
	case 10: draw_xpm(October, x, y); break;
	case 11: draw_xpm(November, x, y); break;
	case 12: draw_xpm(December, x, y); break;*/
	}
}

int calculateFirstWeekDay(View v) {
	// Formula: (year-2000 + day of the month + month code) mod 7

	int w = v.year - 2000 + 1;

	switch(v.month) {
	case 1: w += 6; break;
	case 2: w += 2; break;
	case 3: w += 2; break;
	case 4: w += 5; break;
	case 5: w += 0; break;
	case 6: w += 3; break;
	case 7: w += 5; break;
	case 8: w += 1; break;
	case 9: w += 4; break;
	case 10: w += 6; break;
	case 11: w += 2; break;
	case 12: w += 4; break;
	}

	w = w % 7;

	return w;
	// 0 - Sunday, 1 - Monday, 6 - Saturday
}

void drawMonth(int weekday, int x, int y) {
	switch(weekday) {
	/*case 0: draw_xpm(Sunday, x, y); break;
	case 1: draw_xpm(Monday, x, y); break;
	case 2: draw_xpm(Tuesday, x, y); break;
	case 3: draw_xpm(Wednesday, x, y); break;
	case 4: draw_xpm(Thursday, x, y); break;
	case 5: draw_xpm(Friday, x, y); break;
	case 6: draw_xpm(Saturday, x, y); break;*/
	}
}
