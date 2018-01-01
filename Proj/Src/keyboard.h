#ifndef __KEYBOARD_H
#define __KEYBOARD_H

typedef struct key_press_t{
	unsigned char code;
	unsigned char is2Byte;
	unsigned char mk, bk;
} KEY_PRESS;

int kbd_subscribe_int(void );
int kbd_unsubscribe_int();
struct key_press_t* kbd_int_handler();
int kbd_test_scan();

#endif
