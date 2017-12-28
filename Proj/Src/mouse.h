#ifndef __MOUSE_H
#define __MOUSE_H
#define DELAY_US	20000

typedef struct mouse_action_t{
	unsigned char lmb;
	unsigned char rmb;
	unsigned char mmb;
	char z;
	unsigned int x, y;
} MOUSE_ACTION;

char canIWrite();
char disableMouseInterrupts();
char enableMouseInterrupts();
int kbd_mouse_subscribe_int();
int kbd_mouse_unsubscribe_int();
struct mouse_action_t* kbd_mouse_int_handler();
unsigned char wrt2Mouse(unsigned char cmd, char retransmit);

#endif
