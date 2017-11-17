#include <stdio.h>
#include "vbe.h"
#include "video_gr.c"

void *video_test_init(unsigned short mode, unsigned short delay) {
	printf("Initing mode 0x%x!\n", mode);
	sleep(1);
	void* ret=vg_init(mode);
	printf("vg_init returns 0x%x.\n", ret);


	/*int k, i, j;
	for(k=0;k<64;j++){
		for(i=0;i<1024*768;i++){
			*((char*)ret+i)=j;
		}
		usleep(80000);
	}

*/
	sleep(delay);
	vg_exit();
	return 0;
}


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	/* To be completed */
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
	
