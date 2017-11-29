#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "video.h"

int fill_screen(unsigned short color){
	static video_info_t vi;
	vi = get_vi();


	if(color>63){
		return -1;
	}
	int i;
	for(i=0;i<vi.x*vi.y;i++){
		video_m[i]=color;
	}
	return 0;
}

void setP(unsigned long x, unsigned long y, unsigned char color){
	static video_info_t vi;
	vi = get_vi();

	if(video_m==0){
		printf("Can't write pixel if the screen is not in video mode! Exiting...\n");
		exit(-7);
	}else if(x > vi.x || y > vi.y){
		printf("setP refusing to write outside of the screen!\n");
		exit(-8);
	}
	video_m[y*vi.x+x]=color;

	return;
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
