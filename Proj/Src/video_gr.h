#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H


/** @name VBE Video Info */
/**@{
 *
 * A simplified version of the Packed VBE Mode Info Block
 */
 typedef struct {
	  uint16_t x;      	/**< @brief horizontal resolution in pixels/characters */
	  uint16_t y;      	/**< @brief vertical resolution in pixels/characters */
	  uint8_t bpp; 		/**< @brief bits per pixel */
} video_info_t;

/** @} end of video_info*/


/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputting data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

/**
* @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
*
* @return 0 upon success, non-zero upon failure
*/
int vg_exit(void);

/** @} end of video_gr */

/**
* @brief Returns video_info_t, a simplified version of vbe_mode_info_t.
*
* @return video_info_t
*/
video_info_t get_vi();

/** @} end of get_vi */




#endif /* __VIDEO_GR_H */
