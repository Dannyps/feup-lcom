#ifndef __VIEW_H
#define __VIEW_H

typedef struct {
	int year;
	int month;
	int week;
} View;

void nextMonth(View *v);

void prevMonth(View *v);

void drawMonthName(View v, int x, int y);

/**
 * Returns the day of the week where the first day of the month is
 */
int calculateFirstWeekDay(View v);

void drawMonth(int weekday, int x, int y);

#endif
