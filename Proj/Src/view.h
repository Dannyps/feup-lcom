#ifndef __VIEW_H
#define __VIEW_H

typedef struct {
	int year;
	int month;
	int week;
} View;

void nextMonth(View *v);

void prevMonth(View *v);

void drawMonth(View v, int x, int y);

#endif

