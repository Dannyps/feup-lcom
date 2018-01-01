#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "video.h"
#include "i8042.h"
#include <stdio.h>
#include <stdlib.h>
#include "read_xpm.h"
#include "utils.h"

pixel_t blue_c	=	{0x00, 0x00, 0xff};
pixel_t red_c	=	{0xff, 0x00, 0x00};
pixel_t green_c =	{0x00, 0xff, 0x00};
pixel_t white_c	=	{0xff, 0xff, 0xff};
pixel_t black_c	=	{0x00, 0x00, 0x00};

pixel_t colours[] = {{0x00, 0x00, 0x00}, {0xff, 0x00, 0x00}, {0xff, 0xff, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0xff, 0xff}};

int fill_screen(pixel_t color){

	//printf("filling screen with color %2x%2x%2x.\n", color.r, color.g, color.b);
	static video_info_t vi;
	vi = get_vi();
	//TODO add check if video mode is on
	int i;
	for(i=0; i<vi.x*vi.y*vi.bpp/8 ; i+=3){
		vi.vm[i+0]=color.b;
		vi.vm[i+1]=color.r;
		vi.vm[i+2]=color.g;
	}
	printf("filled screen\n");
	return 0;
}

int rfill_screen(){
	static video_info_t vi;
	vi = get_vi();
	//TODO add check if video mode is on
	int i;
	for(i=0; i<vi.x*vi.y*vi.bpp/8 ; i+=3){
		vi.vm[i+0]=rand()%150+20;
		vi.vm[i+1]=rand()%150;
		vi.vm[i+2]=rand()%50;
	}
	//printf("filled screen\n");
	return 0;
}

void setP(unsigned long x, unsigned long y, pixel_t color){
	//printf("setting pixel %dx%d with color %2x%2x%2x.\n", x,y, color.r, color.g, color.b);
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
	video_m=vg_init(0x118);
}

int draw_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

        // print xpm here

        int width, height;
        char* pix = read_xpm(xpm, &width, &height);
        char* tPix=pix;
        if(pix==NULL){
        	//failed
        	vg_exit();
        	exit(-3);
        }
        width += xi;
        height += yi;
        int initial_x = xi;
        int xpm_counter = 0;
        // printing
        while(yi < height) {
			xi = initial_x;
			while(xi < width) {
				// colours[pix[xpm_counter]]
				pixel_t tempColor;
				memcpy(&tempColor, tPix, 3);
				setP(xi, yi, tempColor);
				xi++;
				xpm_counter++;
				tPix+=3;
			}
			yi++;
        }
        free(pix);
        return 0;
}



int draw_xpm_from_memory(xpm_t xpm, unsigned short xi, unsigned short yi) {
        int width, height;
        width=xpm.width;
        height=xpm.height;
        char* pix = xpm.pointer;
        char* tPix=pix;
        if(pix==NULL){
        	//failed
        	vg_exit();
        	exit(-3);
        }
        width += xi;
        height += yi;
        int initial_x = xi;
        int xpm_counter = 0;
        // printing
        while(yi < height) {
			xi = initial_x;
			while(xi < width) {
				// colours[pix[xpm_counter]]
				pixel_t tempColor;
				memcpy(&tempColor, tPix, 3);
				setP(xi, yi, tempColor);
				xi++;
				xpm_counter++;
				tPix+=3;
			}
			yi++;
        }
        return 0;
}


void draw_cursor(unsigned x, unsigned y){

	//printf("Drawing cursor on %dx%d.\n", x, y);
	unsigned i, j;
	for(i=x;i<x+5;i++){
		for(j=y;j<y+5;j++){
			setP(i, j, red_c);
		}
	}
}

void draw_character(char asciiCode, unsigned short x, unsigned short y, pixel_t color){
	/**
	 * x and y mark the bottom left corner.
	 */

	if(asciiCode<32 || asciiCode > 127){
		fprintf(cerr, "could not write unprintable character ASCII: %d!\n", asciiCode);
		return ;
	}

	video_info_t vi = get_vi();
	if(x<0 || y-8<0 || x+13>vi.x || y>vi.y){
		fprintf(cerr, "could not write character out of screen (on %dx%d)!\n", x, y);
		return;
	}

	// we've checked our args, they look good to go.

	/**
	 * A small crash course on our letters mini pixmaps is in order:
	 *
	 * - a bit set to 1 represents a pixel to be filled with the color passed in the args.
	 * - a bit set to 0 should be left untouched. Another function must set the background before.
	 * - every char is 13 pixels in height, and 8 pixels in width (monospace), thus occupying 104 pixels.
	 * - that's all folks!
	 */


	unsigned short ax, ay; // actual x and actual y, because we start drawing from the top-left corner.
	ax=x;
	ay=y-8;

	int i, j;
	char* a=letters[asciiCode];


}
