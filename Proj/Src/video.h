
#ifndef __VIDEO_H
#define __VIDEO_H

#include "video_gr.h"

char* video_m;

typedef struct{
	unsigned char r,g,b;
} pixel_t;

extern pixel_t blue_c;
extern pixel_t red_c;
extern pixel_t green_c;
extern pixel_t white_c;
extern pixel_t black_c;

int fill_screen(pixel_t color);
void setP(unsigned long x, unsigned long y, pixel_t color);
void video_start();
#endif
