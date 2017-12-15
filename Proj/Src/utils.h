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


void start_listening();
void draw_main_page();


#endif
