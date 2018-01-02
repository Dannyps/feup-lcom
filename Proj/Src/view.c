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

xpm_t* getXPMByMonth(unsigned month){
	switch(month) {
		case 1: return &XPM_january;
		case 2: return &XPM_february;
		case 3: return &XPM_march;
		case 4: return &XPM_april;
		case 5: return &XPM_may;
		case 6: return &XPM_june;
		case 7: return &XPM_july;
		case 8: return &XPM_august;
		case 9: return &XPM_september;
		case 10: return &XPM_october;
		case 11: return &XPM_november;
		case 12: return &XPM_december;
	}
	return NULL;
}

// https://github.com/FEUP-MIEEC/Prog1/blob/master/Aula_Pratica_03/Problema_8.c
/**
 * @brief If year if bissextile or not.
 */
int bissextile(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0)
                return 1;
        } else {
            return 1;
        }
    }
    return 0;
}

unsigned short getDaysInMonth(unsigned month, unsigned year){
	switch(month){
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
		break;
	case 2:
		if(bissextile(year))
			return 27;
		else
			return 28;
		break;
	}
	return 0;
}

void nextYear(View *v){
	v->year++;
	v->xpm = getXPMByMonth(v->month);
}

void prevYear(View *v){
	v->year--;
	v->xpm = getXPMByMonth(v->month);
}

void nextMonth(View *v) {
	if(v->month != 12) v->month += 1;
	else {
		v->month = 1;
		v->year += 1;
	}

	v->xpm = getXPMByMonth(v->month);
}

void prevMonth(View *v) {
	if(v->month != 1) v->month -= 1;
	else {
		v->month = 12;
		v->year -= 1;
	}

	v->xpm = getXPMByMonth(v->month);
}

void drawMonthName(View *v, int x, int y) {
	v->daysInTheMonth = getDaysInMonth(v->month, v->year);
	draw_xpm_from_memory(*(v->xpm), x, y);
}

int calculateFirstWeekDay(View *v) {
	// Formula: (year-2000 + day of the month + month code) mod 7
	unsigned a, y, m;
	a = (14 - v->month) / 12;
	y = v->year - a;
	m = v->month + (12 * a) - 2;

	return ((y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7);
}


xpm_t* getXPMByWeekDay(unsigned wd){
	switch(wd) {
		case 0: return &XPM_monday;
		case 1: return &XPM_tuesday;
		case 2: return &XPM_wednesday;
		case 3: return &XPM_thursday;
		case 4: return &XPM_friday;
		case 5: return &XPM_saturday;
		case 6: return &XPM_sunday;
	}
	return NULL;
}

struct marker{
	unsigned x, y;
};

void cover_surplus_days(View *v, unsigned short x, unsigned short y){
	struct marker d27, d28, d29, d30, d31;
	switch(calculateFirstWeekDay(v)) {
		case 0:
			d27.x=400/7*5; 	d27.y=150;
			d28.x=400/7*6;	d28.y=150;
			d29.x=400/7*0;	d29.y=200;
			d30.x=400/7*1;	d30.y=200;
			d31.x=400/7*2;	d31.y=200;
			break;
		case 1:
			d27.x=400/7*6; 	d27.y=150;
			d28.x=400/7*0;	d28.y=200;
			d29.x=400/7*1;	d29.y=200;
			d30.x=400/7*2;	d30.y=200;
			d31.x=400/7*3;	d31.y=200;
			break;
		case 2:
			d27.x=400/7*0;	d27.y=200;
			d28.x=400/7*1;	d28.y=200;
			d29.x=400/7*2;	d29.y=200;
			d30.x=400/7*3;	d30.y=200;
			d31.x=400/7*4;	d31.y=200;
			break;
		case 3:
			d27.x=400/7*1;	d27.y=200;
			d28.x=400/7*2;	d28.y=200;
			d29.x=400/7*3;	d29.y=200;
			d30.x=400/7*4;	d30.y=200;
			d31.x=400/7*5;	d31.y=200;
			break;
		case 4:
			d27.x=400/7*2;	d27.y=200;
			d28.x=400/7*3;	d28.y=200;
			d29.x=400/7*4;	d29.y=200;
			d30.x=400/7*5;	d30.y=200;
			d31.x=400/7*6;	d31.y=200;
			break;
		case 5:
			d27.x=400/7*3;	d27.y=200;
			d28.x=400/7*4;	d28.y=200;
			d29.x=400/7*5;	d29.y=200;
			d30.x=400/7*6;	d30.y=200;
			d31.x=400/7*0;	d31.y=250;
			break;
		case 6:
			d27.x=400/7*4;	d27.y=200;
			d28.x=400/7*5;	d28.y=200;
			d29.x=400/7*6;	d29.y=200;
			d30.x=400/7*0;	d30.y=250;
			d31.x=400/7*1;	d31.y=250;
			break;
	}

	if(v->daysInTheMonth<28){
		draw_box(x+d28.x, y+d28.y, 400/7, 50, white_c);
	}
	if(v->daysInTheMonth<29){
		draw_box(x+d29.x, y+d29.y, 400/7, 50, white_c);
	}
	if(v->daysInTheMonth<30){
		draw_box(x+d30.x, y+d30.y, 400/7, 50, white_c);
	}
	if(v->daysInTheMonth<31){
		draw_box(x+d31.x, y+d31.y, 400/7, 50, white_c);
	}
}


void drawMonth(View *v, int x, int y) {
	int w=0;
	w = calculateFirstWeekDay(v);
	xpm_t* xpm;
	xpm = getXPMByWeekDay(w);
	draw_xpm_from_memory(*xpm, x, y);
	char yearStr[5];
	sprintf(yearStr, "%d", v->year);
	draw_string(yearStr, x+350, y-80, black_c);

	// gotta blank extra days
	cover_surplus_days(v, x, y);
}


