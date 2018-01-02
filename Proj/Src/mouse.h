#ifndef __MOUSE_H
#define __MOUSE_H

/** @defgroup timer timer
 * @{
 *
 * @brief Functions for interfacing with the mouse.
 */

#define DELAY_US	20000

/**
 * @brief a struct to simplify handling mouse interrupts
 */
typedef struct mouse_action_t{
	unsigned char lmb; /**< @brief if the left mouse button was being pressed */
	unsigned char rmb; /**< @brief if the right mouse button was being pressed */
	unsigned char mmb; /**< @brief if the middle mouse button was being pressed */
	char z; /**< @brief scrolling wheel displacement */
	int x; /**< @brief movement on x */
	int y; /**< @brief movement on y */
} MOUSE_ACTION;

extern unsigned cursorX, cursorY;

/**
 * @brief Verifies if the mouse is ready to receive a command
 * @return 0 on success, non-zero otherwise
 */
char canIWrite();

/**
 * @brief subscribes mouse interrupts from the IRQ.
 * @return 0 on success, non-zero otherwise
 */
int kbd_mouse_subscribe_int();

/**
 * @brief unsubscribes mouse interrupts from the IRQ.
 * @return 0 on success, non-zero otherwise
 */
int kbd_mouse_unsubscribe_int();

/**
 * @brief mouse driver interrupt handler.
 * @return pointer to MOUSE_ACTION.
 *
 * @warning Returned structure should be freed asap!
 */
MOUSE_ACTION* kbd_mouse_int_handler();

/**
 * @brief writes to mouse, and retries on failure
 * @param cmd command to be sent
 * @param retransmit if the command should be retransmitted on failure
 * @return 0 on success, non-zero otherwise
 */
unsigned char wrt2Mouse(unsigned char cmd, char retransmit);

/**
 * @brief determines if mouse is in the passed box
 * @param tlX top-left x value
 * @param tlY top-left y value
 * @param brX bottom-right x value
 * @param bottom-right y value
 * @return 0 on success, non-zero otherwise
 */
int mouseInBox(unsigned tlX, unsigned tlY, unsigned brX, unsigned brY);

/**
 * @brief verifies if the mouse has been clicked inside one of the current boxes on the screen, and takes appropriate action.
 * @param ma mouse action struct
 */
void check_clicks(struct mouse_action_t* ma);

/**
 * @brief Updates global variable with new mouse coordinates upon mouse movement.
 * @param ma mouse action struct
 */
void calc_new_mouse_coords(struct mouse_action_t* ma);

/**}*/
#endif
