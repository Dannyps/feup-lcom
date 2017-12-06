#ifndef __VIEW_H
#define __VIEW_H

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
	void (*printView)();

	int (*getYear)();
	void (*setYear)();
	int (*getWeek)();
	void (*setWeek)();

	int year, month, week;
} View;

View View();

static void setYear(View *this, int year) {
    this->year = year;
}

static void setMonth(View *this, int month) {
    this->month = month;
}

static void setWeek(View *this, int week) {
    this->week = week;
}

static int getYear(View *this) {
    return this->year;
}

#endif
