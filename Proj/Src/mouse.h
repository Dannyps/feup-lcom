#ifndef __MOUSE_H
#define __MOUSE_H
#define DELAY_US	20000

typedef struct mouse_action_t{
	unsigned char lmb;
	unsigned char rmb;
	unsigned char mmb;
	unsigned char xov;
	unsigned char yov;
	unsigned int X, Y;
} MOUSE_ACTION;

int lhid, khid;
int timer0_hookIDs[2];
int seconds_elapsed, ticks_elapsed;
int length_ctr;

char canIWrite();
char disableMouseInterrupts();
char enableMouseInterrupts();
int kbd_mouse_subscribe_int(void );
int kbd_mouse_unsubscribe_int();
struct mouse_action_t* kbd_mouse_int_handler(int gesture);
unsigned char wrt2Mouse(unsigned char cmd, char retransmit);

#endif
