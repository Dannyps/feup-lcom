#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/** @defgroup keyboard keyboard
 * @{
 *
 * @brief Functions for interfacing with the keyboard.
 */


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

/**
 * @brief Subscribes KB interrupts from the IRQ.
 * @return 0 on success, non-zero otherwise
 */
int kbd_subscribe_int();

/**
 * @brief Unsubscribes KB interrupts from the IRQ.
 * @return 0 on success, non-zero otherwise
 */
int kbd_unsubscribe_int();

/**
 * @brief Keyboard driver interrupt handler.
 * @return Pointer to KEY_PRESS.
 *
 * @warning Returned structure should be freed asap!
 */
KEY_PRESS* kbd_int_handler();

/**@}*/


#endif
