#ifndef __VIEW_H
#define __VIEW_H
#include "read_xpm.h"

typedef struct view{
	int year;
	int month;
	int week;
	int daysInTheMonth;
} View;

xpm_t XPM_january;
xpm_t XPM_february;
xpm_t XPM_march;
xpm_t XPM_april;
xpm_t XPM_may;
xpm_t XPM_june;
xpm_t XPM_july;
xpm_t XPM_august;
xpm_t XPM_september;
xpm_t XPM_october;
xpm_t XPM_november;
xpm_t XPM_december;

xpm_t XPM_monday;
xpm_t XPM_tuesday;
xpm_t XPM_wednesday;
xpm_t XPM_thursday;
xpm_t XPM_friday;
xpm_t XPM_saturday;
xpm_t XPM_sunday;

void nextMonth(View *v);

void prevMonth(View *v);

void drawMonthName(View *v, int x, int y);

/**
 * Returns the day of the week where the first day of the month lands
 */
int calculateFirstWeekDay(View *v);

void drawMonth(View *v, int x, int y);

#endif
