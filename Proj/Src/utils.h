#ifndef __UTILS_H
#define __UTILS_H

#include "keyboard.h"

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
extern char srchStr[64];
extern int stop;
extern int landing;
extern int search;

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
 * @param folder the absolute path to the project folder.
 */
void load_xpms(char* folder);

/**
 * @brief draws landing page until a key is pressed.
 */
void draw_landing_page();

/**
 * @brief Takes care of received text, adding characters, or deleting them when a backspace is received.
 */
void textInput(KEY_PRESS* kp);

/**
 * @brief Draws the searchbox on the video buffer.
 */
void draw_search_box();

int min(int a, int b);

int max(int a, int b);

int interpretSrchStr();
/** @} */

extern const unsigned char letters[95][13];
extern const unsigned char kbCodes[13];

#endif
