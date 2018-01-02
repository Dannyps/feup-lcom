#ifndef __UTILS_H
#define __UTILS_H
#define BIT(n) (0x01<<(n))

/*  GLOBAL VARIABLES  */

/*
 *  Hook IDs structure
 *
 * int[2];
 *  _____________________________________________
 * |          0           |           1          |
 * |----------------------|----------------------|
 * | lhid (local hook id) | khid (kernel hook id)|
 * |______________________|______________________|
 *
 * */

int timer0_hookIDs[2];
int kbc_hookIDs[2];
int mouse_hookIDs[2];
int rtc_hookIDs[2];

extern char rtcStr[64];
extern int stop;

/** @defgroup init init
 * @{
*/

/**
 * @brief The function that calls the subscribers and runs the so-called <i>not polling</i> loop.
 */
void start_listening();

/**
 *
 * @brief The function that prints the main page.
 * @pre load_xpms()
 */
void draw_main_page();

/**
 * @brief The function that reads the xpms to memory.
 */
void load_xpms();

/**}*/

extern const unsigned char letters[95][13];

#endif
