#ifndef __VIEWMONTH_H
#define __VIEWMONTH_H

#include <stdio.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <machine/int86.h>
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

typedef struct {
	//struct View;

	void (*printView)();
	char *(*getName)();
	char* month_names = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
	void (*get_numDays)();
	int numDays;
	int (*getMonth)();
	void (*setMonth)();
	int month;
} ViewMonth;

//ViewMonth ViewMonth();

static char* getName(ViewMonth *this) {
	return this->month_names[this->month];
}

static void get_numDays(ViewMonth *this) {
	return this->numDays;
}

static int getMonth(View *this) {
    return this->month;
}

static int getWeek(View *this) {
    return this->week;
}

#endif
