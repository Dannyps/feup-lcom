#include <stdio.h>
#include "vbe.h"
#include "video_gr.c"

char* video_m;

void setP(unsigned long x, unsigned long y, unsigned char color){
	video_m[y*H_RES+x]=color;
}


void video_start(){
	video_m=vg_init(0x105);
}

void *video_test_init(unsigned short mode, unsigned short delay) {
	printf("Initing mode 0x%x!\n", mode);

	video_m=vg_init(mode);

	sleep(delay);

	vg_exit();
	return 0;
}


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	


	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	/* To be completed */
	return 0;
}
	
int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {
	
	/* To be completed */
	return 0;
}	

int test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	/* To be completed */
	return 0;
}	

int test_controller() {

	/* To be completed */
	return 0;
}	
	
