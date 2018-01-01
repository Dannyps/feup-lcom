#ifndef __VIEW_H
#define __VIEW_H
#include "read_xpm.h"


/** @defgroup view view
 * @{
 *
 * @brief structs and functions to handle the calendar generation.
 */

/**
 * @brief relevant details to a view
 */
typedef struct view{
	int year; 			/**< @brief current year*/
	int month;			/**< @brief current month*/
	int daysInTheMonth;	/**< @brief days in the current month/year pair*/
	xpm_t* xpm;			/**< @brief pointer to the xpm associated with the current month name*/
} View;

/**
 * @brief relevant XPMs
 */
extern xpm_t XPM_exitCross, XPM_january, XPM_february, XPM_march, XPM_april, XPM_may, XPM_june; // camelCase please!
extern xpm_t XPM_july, XPM_august, XPM_september, XPM_october, XPM_november, XPM_december;
extern xpm_t XPM_monday, XPM_tuesday, XPM_wednesday, XPM_thursday, XPM_friday, XPM_saturday, XPM_sunday;

/**
 * @brief advance 1 year
 * @param pointer to the View
 * @return void
 */
void nextMonth(View *v);
void prevMonth(View *v);

void nextYear(View *v);
void prevYear(View *v);

void drawMonthName(View *v, int x, int y);

/**
 * Returns the day of the week where the first day of the month lands
 */
int calculateFirstWeekDay(View *v);

void drawMonth(View *v, int x, int y);

#endif
