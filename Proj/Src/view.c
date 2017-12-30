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
#include "utils.h"
#include "read_xpm.h"

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

void drawMonthName(View *v, int x, int y) {
	switch(v->month) {
	case 1: v->daysInTheMonth = 31;
		draw_xpm_from_memory(XPM_january, x, y); break;
	case 2: v->daysInTheMonth = 28;
		draw_xpm_from_memory(XPM_february, x, y); break;
	case 3: v->daysInTheMonth = 31;
		draw_xpm_from_memory(XPM_march, x, y); break;
	/*	case 4: v->daysInTheMonth = 30;
	 * draw_xpm_from_memory(XPM_april, x, y); break;
		case 5: v->daysInTheMonth = 31;
		draw_xpm_from_memory(XPM_may, x, y); break;
		case 6: v->daysInTheMonth = 30;
		draw_xpm_from_memory(XPM_june, x, y); break;
		case 7: v->daysInTheMonth = 31;
		draw_xpm_from_memory(XPM_july, x, y); break;
		case 8: v->daysInTheMonth = 31;
		draw_xpm_from_memory(XPM_august, x, y); break;
		case 9: v->daysInTheMonth = 30;
		draw_xpm_from_memory(XPM_september, x, y); break;
		case 10: v->daysInTheMonth = 31;
		draw_xpm_from_memory(XPM_october, x, y); break;
		case 11: v->daysInTheMonth = 30;
		draw_xpm_from_memory(XPM_november, x, y); break;
		case 12: v->daysInTheMonth = 31;
		draw_xpm_from_memory(XPM_december, x, y); break;
	 */	}


}

int calculateFirstWeekDay(View *v) {
	// Formula: (year-2000 + day of the month + month code) mod 7

	int w = v->year - 2000 + 1;

	switch(v->month) {
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

	w=0;
	return w;
}

void drawMonth(View *v, int x, int y) {
	int w=0;
	w = calculateFirstWeekDay(v);

	switch(w) {
	case 0: draw_xpm_from_memory(XPM_monday, x, y);
	/*	case 1: draw_xpm_from_memory(XPM_thursday, x, y);
	case 2: draw_xpm_from_memory(XPM_wednesday, x, y);
	case 3: draw_xpm_from_memory(XPM_thursday, x, y);
	case 4: draw_xpm_from_memory(XPM_friday, x, y);
	case 5: draw_xpm_from_memory(XPM_saturday, x, y);
	case 6: draw_xpm_from_memory(XPM_sunday, x, y);
	 */	}
}


