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

	switch(v->month) {
	case 1: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/january.xpm"; break;
	case 2: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/february.xpm"; break;
	case 3: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/march.xpm"; break;
	case 4: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/april.xpm"; break;
	case 5: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/may.xpm"; break;
	case 6: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/june.xpm"; break;
	case 7: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/july.xpm"; break;
	case 8: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/august.xpm"; break;
	case 9: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/september.xpm"; break;
	case 10: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/october.xpm"; break;
	case 11: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/november.xpm"; break;
	case 12: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/december.xpm"; break;
	}
}

void prevMonth(View *v) {
	if(v->month != 1) v->month -= 1;
	else {
		v->month = 12;
		v->year -= 1;
	}

	switch(v->month) {
	case 1: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/january.xpm"; break;
	case 2: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/february.xpm"; break;
	case 3: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/march.xpm"; break;
	case 4: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/april.xpm"; break;
	case 5: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/may.xpm"; break;
	case 6: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/june.xpm"; break;
	case 7: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/july.xpm"; break;
	case 8: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/august.xpm"; break;
	case 9: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/september.xpm"; break;
	case 10: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/october.xpm"; break;
	case 11: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/november.xpm"; break;
	case 12: v->monthFilename = "/home/lcom/Proj/Src/xpms/months/december.xpm"; break;
	}
}

void drawMonthName(View *v, int x, int y) {
	switch(v->month) {
	case 1:
		v->daysInTheMonth = 31;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 2:
		v->daysInTheMonth = 28;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 3:
		v->daysInTheMonth = 31;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 4:
		v->daysInTheMonth = 30;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 5:
		v->daysInTheMonth = 31;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 6:
		v->daysInTheMonth = 30;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 7:
		v->daysInTheMonth = 31;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 8:
		v->daysInTheMonth = 31;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 9:
		v->daysInTheMonth = 30;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 10:
		v->daysInTheMonth = 31;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 11:
		v->daysInTheMonth = 30;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	case 12:
		v->daysInTheMonth = 31;
		XPM_month = read_xpm_from_file(v->monthFilename);
		draw_xpm_from_memory(XPM_month, x, y); break;
	}
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
	case 0:
		XPM_weekday = read_xpm_from_file("/home/lcom/Proj/Src/xpms/monthviews/monday.xpm");
		draw_xpm_from_memory(XPM_weekday, x, y);
		break;
	case 1:
		XPM_weekday = read_xpm_from_file("/home/lcom/Proj/Src/xpms/monthviews/tuesday.xpm");
		draw_xpm_from_memory(XPM_weekday, x, y);
		break;
	case 2:
		XPM_weekday = read_xpm_from_file("/home/lcom/Proj/Src/xpms/monthviews/wednesday.xpm");
		draw_xpm_from_memory(XPM_weekday, x, y);
		break;
	case 3:
		XPM_weekday = read_xpm_from_file("/home/lcom/Proj/Src/xpms/monthviews/thursday.xpm");
		draw_xpm_from_memory(XPM_weekday, x, y);
		break;
	case 4:
		XPM_weekday = read_xpm_from_file("/home/lcom/Proj/Src/xpms/monthviews/friday.xpm");
		draw_xpm_from_memory(XPM_weekday, x, y);
		break;
	case 5:
		XPM_weekday = read_xpm_from_file("/home/lcom/Proj/Src/xpms/monthviews/saturday.xpm");
		draw_xpm_from_memory(XPM_weekday, x, y);
		break;
	case 6:
		XPM_weekday = read_xpm_from_file("/home/lcom/Proj/Src/xpms/monthviews/sunday.xpm");
		draw_xpm_from_memory(XPM_weekday, x, y);
		break;
	}
}


