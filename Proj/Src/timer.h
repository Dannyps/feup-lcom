#ifndef __TIMER_H
#define __TIMER_H

int timer0_hookIDs[2];
int timer0_subscribe_int(void );
int timer0_unsubscribe_int();
void timer0_int_handler();

#endif
