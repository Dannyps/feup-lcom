#ifndef __MOUSE_H
#define __MOUSE_H

char canIWrite();
char disableMouseInterrupts();
char enableMouseInterrupts();
int kbd_mouse_subscribe_int(void );
int kbd__mouse_unsubscribe_int();
void kbd_mouse_int_handler(int gesture);
unsigned char wrt2Mouse(unsigned char cmd, char retransmit);

#endif
