#ifndef __KEYBOARD_H
#define __KEYBOARD_H

typedef struct key_press_t{
	unsigned char code; /**< @brief the code generated, without 0xe0 prefix */
	unsigned char is2Byte; /**< @brief wether the 0xe0 byte was present */
	unsigned char mk, bk; /**< @brief is makecode/breakcode */
} KEY_PRESS;

int kbd_subscribe_int(void );
int kbd_unsubscribe_int();
struct key_press_t* kbd_int_handler();
int kbd_test_scan();

#endif
