#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup timer timer
 * @{
 *
 * @brief Functions for interfacing with the timer.
 */


int timer0_hookIDs[2];

/**
 * @brief subscribes RTC interrupts from the timer.
 *
 * @return 0 on success, non-zero otherwise
 */
int timer0_subscribe_int();

/**
 * @brief unsubscribes RTC interrupts from the timer.
 *
 * @return 0 on success, non-zero otherwise
 */
int timer0_unsubscribe_int();

/**
 * @brief handles the interrupt. Refreshes the video buffer and calls vg_flush().
 */
void timer0_int_handler();

/**@}*/

#endif
