#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC. Needs to be completed.
 */


#define BIT(n) (0x01<<(n))

#define KBC_IRQ	        1    /**< @brief KBC IRQ line */

/* I/O port addresses */

#define OUT_BUF			0x60 /**< @brief kbc output buffer */
#define IN_BUF			0x60 /**< @brief kbc input buffer */

#define STATUS_REG		0x64 /**< @brief kbc input buffer */

/**@}*/

#endif /* _LCOM_I8254_H */

