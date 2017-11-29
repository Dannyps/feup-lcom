#ifndef __VIDEO_H
#define __VIDEO_H

char* video_m;

int fill_screen(unsigned short color);
void setP(unsigned long x, unsigned long y, unsigned char color);
void video_start();
void *video_test_init(unsigned short mode, unsigned short delay);

#endif
