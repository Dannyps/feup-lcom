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

/*
void printMonth(View v, int x, int y) {
	test_xpm(month_names[v.month], x, y);
}*/

#endif
