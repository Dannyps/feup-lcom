#ifndef __RTC_H
#define __RTC_H

/** @defgroup rtc rtc
 * @{
 *
 * @brief Constants and macros for interfacing with the RTC.
 */
#define BIT(n) (0x01<<(n))

#define RTC_ADDR_REG 0x70 /**< @brief RTC Address Register */
#define RTC_DATA_REG 0x71 /**< @brief RTC Data Register*/

#define RTC_IRQ_LINE /**< @brief RTC IRQ Line*/

// RTC Internal Address Space
#define RTC_ADDRESS_SECONDS 		0
#define RTC_ADDRESS_SECONDS_ALARM 	1
#define RTC_ADDRESS_MINUTES			2
#define RTC_ADDRESS_MINUTES_ALARM	3
#define RTC_ADDRESS_HOURS 			4
#define RTC_ADDRESS_HOURS_ALARM 	5
#define RTC_ADDRESS_DAY_WEEK 		6
#define RTC_ADDRESS_DAY_MONTH		7
#define RTC_ADDRESS_MONTH			8
#define RTC_ADDRESS_YEAR			9
#define RTC_CTRL_REG_A 				10
#define RTC_CTRL_REG_B  			11
#define RTC_CTRL_REG_C  			12
#define RTC_CTRL_REG_D 				13

#define RTC_UIP						BIT(7)  /**< @brief Update In Progress bit selector*/

/**
 * @brief Struct to manage a read from the RTC.
 *
 */
typedef struct {
	unsigned char hour; /**< @brief hour*/
	unsigned char min; 	/**< @brief minutes*/
	unsigned char sec;	/**< @brief seconds*/
	unsigned char day;	/**< @brief day of the month*/
	unsigned char month;/**< @brief month*/
	unsigned char year;	/**< @brief year (only two last digits)*/
	unsigned char wd;	/**< @brief day of the week*/
} rtc_time_t;

/**
 * @brief subscribes RTC interrupts from the IRQ.
 *
 * @note also enables update interrupts in the control register of the RTC
 *
 * @return 0 on success, non-zero otherwise
 */
int rtc_subscribe_int();

/**
 * @brief unsubscribes RTC interrupts from the IRQ.
 *
 * @note also disables update interrupts in the control register of the RTC
 *
 * @return 0 on success, non-zero otherwise
 */
int rtc_unsubscribe_int();

void wait_for_valid_rtc();

/**
 * @brief reads time from the RTC registers.
 *
 * @note should only be called when a UIE is received.
 * @note returns values in binary, as they are converted from bcd beforehand.
 *
 * @return pointer to rtc_time_t. This should be freed asap.
 */
rtc_time_t* rtc_int_handler();

/**@}*/

#endif
