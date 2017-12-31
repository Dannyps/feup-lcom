#ifndef __VIEW_H
#define __VIEW_H
#include "read_xpm.h"

typedef struct view{
	int year;
	int month;
	int daysInTheMonth;
	char* monthFilename;
	int daysInTheMonth;
} View;

xpm_t XPM_month;
xpm_t XPM_weekday;

void nextMonth(View *v);

void prevMonth(View *v);

void drawMonthName(View *v, int x, int y);

/**
 * Returns the day of the week where the first day of the month lands
 */
int calculateFirstWeekDay(View *v);

void drawMonth(View *v, int x, int y);

#endif
