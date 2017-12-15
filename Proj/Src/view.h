#ifndef __VIEW_H
#define __VIEW_H

#include <sys/time.h>
#include <time.h>

#include "vbe.h"
#include "video_gr.h"
#include "i8042.h"
#include "i8254.h"
#include <minix/sysutil.h>
#include "lmlib.h"

#include "keyboard.h"
#include "timer.h"
#include "video.h"
#include "mouse.h"

char* month_names[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

typedef struct {
	int year;
	int month;
	int week;
} View;

void nextMonth(View v) {
	if(v.month != 12) v.month += 1;
	else {
		v.month = 1;
		v.year += 1;
	}
}

void prevMonth(View v) {
	if(v.month != 1) v.month -= 1;
	else {
		v.month = 12;
		v.year -= 1;
	}
}

/*
char* getMonth(View v) {
	char *ret;
	switch(v.month) {
	case 1: return January;
	case 2: return February;
	case 3: return March;
	case 4: return April;
	case 5: return May;
	case 6: return June;
	case 7: return July;
	case 8: return August;
	case 9: return September;
	case 10: return October;
	case 11: return November;
	case 12: return December;
	case default: break;
	}
}*/

#endif
