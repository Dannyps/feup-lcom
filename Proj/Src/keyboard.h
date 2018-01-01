#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/**
 * @brief Struct to manage a received interrupt from the keyboard.
 *
 * @note mk is always the compliment of bk.
 */
typedef struct key_press_t{
	unsigned char code; /**< @brief the code generated, without the 0xe0 prefix */
	unsigned char is2Byte; /**< @brief whether the 0xe0 byte was present */
	unsigned char mk; /**< @brief 1 if is makecode, 0 otherwise */
	unsigned char bk; /**< @brief 1 if is breakcode, 0 otherwise */
} KEY_PRESS;

int kbd_subscribe_int(void );
int kbd_unsubscribe_int();
struct key_press_t* kbd_int_handler();
int kbd_test_scan();

#endif
