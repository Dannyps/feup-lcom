#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * @brief Constants for programming the i8042 KBC.
 */


#define BIT(n) (0x01<<(n))

#define KBC_IRQ	        1    /**< @brief KBC IRQ line */
#define MOUSE_IRQ       12   /**< @brief Mouse IRQ line */

/* I/O port addresses */

#define OUT_BUF			0x60 /**< @brief kbc output buffer */
#define IN_BUF			0x60 /**< @brief kbc input buffer */

#define STATUS_REG		0x64 /**< @brief kbc input buffer */
#define CTRL_REG        0x64 /**< @brief kbc control register */

/* KBC COMMANDS */

#define WRITE_BYTE                0xD4        /**< @brief kbc WRITE BYTE command, called b4 mouse commands */

#define READ_CMD_BYTE        0x20

#define WRITE_CMD_BYTE        0x60

/* MOUSE COMMANDS */

#define ENABLE_STREAM_MODE        0xF4        /**< @brief mouse: enable stream mode */

#define DISABLE_STREAM_MODE        0xF5        /**< @brief mouse: disable stream mode */

#define SET_STREAM_MODE                0xEA        /**< @brief mouse: set stream mode */

#define SET_REMOTE_MODE                0xF0        /**< @brief mouse: set remote mode */

#define READ_DATA                        0xEB        /**< @brief mouse: read data from remote mode */

/* Mouse controller -- Acknowledgment bytes */

#define MOUSE_ACK                0xFA        /**< @brief mouse response: everything is ok */

#define MOUSE_NACK                0xFE        /**< @brief mouse response: invalid byte */

#define MOUSE_ERROR                0xFC        /**< @brief mouse response: second consecutive invalid byte */

/**@}*/

#endif /* _LCOM_I8254_H */

