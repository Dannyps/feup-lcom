#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "video.h"

int fill_screen(pixel_t color){
	printf("filling screen with color %2x%2x%2x.\n", color.r, color.g, color.b);
	static video_info_t vi;
	vi = get_vi();
	//TODO add check if video mode is on
	int i;
	for(i=0; i<vi.x*vi.y*vi.bpp/8 ; i+=3){
		vi.vm[i+0]=color.b;
		vi.vm[i+1]=color.r;
		vi.vm[i+2]=color.g;
	}
	printf("done\n");
	return 0;
}

void setP(unsigned long x, unsigned long y, pixel_t color){
	static video_info_t vi;
	vi = get_vi();
	if(vi.vm==0){
		printf("Can't write pixel if the screen is not in video mode! Exiting...\n");
		exit(-7);
	}else if(x > vi.x || y > vi.y){
		printf("setP refusing to write outside of the screen!\n");
		exit(-8);
	}
	unsigned pos=(y*vi.x+x)*vi.bpp/8;
	vi.vm[pos+0]=color.g;
	vi.vm[pos+1]=color.b;
	vi.vm[pos+2]=color.r;
	return;
}

void video_start(){
	video_m=vg_init(0x105);
}
