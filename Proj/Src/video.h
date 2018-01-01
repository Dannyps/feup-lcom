#ifndef __VIDEO_H
#define __VIDEO_H

#include "video_gr.h"
#include "keyboard.h"
#include "vbe.h"
#include "read_xpm.h"


/** @defgroup video video
 * @{
 *
 * @brief higher level functions for interfacing with the video buffer.
 */
/**
 * @brief pointer to the video memory.
 */
char* video_m;

/**
 * @brief pixel containing r,g,b values (0-255 each)
 */
typedef struct{
	unsigned char r,g,b;
} pixel_t;

extern pixel_t blue_c;
extern pixel_t red_c;
extern pixel_t green_c;
extern pixel_t white_c;
extern pixel_t black_c;

/**
 * @brief fill screen with the specified color
 * @param color to be used
 * @return 0 on success, non-zero otherwise.
 */
int fill_screen(pixel_t color);

/**
 * @brief fill screen with pseudo-random color
 * @return 0 on success, non-zero otherwise.
 */
int rfill_screen();

/**
 * @brief set a certain pixel with the specified color
 *
 * the most important function in the project!!
 * @note attempt to write out of the screen will result in the program exiting quasi-successfully. Use with caution.
 * @param x
 * @param y
 * @param color
 */
void setP(unsigned long x, unsigned long y, pixel_t color);

/**
 * @brief start video mode 0x118.
 */
void video_start();

/**
 * @brief draw the specified xpm on the video memory.
 * @param the xpm to be drawn
 * @return 0 on success, non-zero otherwise.
 */
int draw_xpm(char *xpm[], unsigned short xi, unsigned short yi);

/**
 * @brief draw the cursor on the specified variables.
 */
void draw_cursor(unsigned x, unsigned y);
int draw_xpm_from_memory(xpm_t xpm, unsigned short xi, unsigned short yi);
void draw_character(char asciiCode, unsigned short x, unsigned short y, pixel_t color);
void draw_string(char *str, short unsigned x, short unsigned y, pixel_t color);
unsigned short get_string_width(char* str);
void draw_box(unsigned short x, unsigned short y, unsigned short width, unsigned short height, pixel_t color);
#endif
