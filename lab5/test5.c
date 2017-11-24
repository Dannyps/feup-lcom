#include <stdio.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>

#include <sys/time.h>
#include <time.h>

#include "vbe.h"
#include "video_gr.h"
#include "i8042.h"
#include <minix/sysutil.h>
#include "read_xpm.c"
#include "pixmap.h"
#include "video_test.h"

// KEYBOARD FUNCTIONS //

int kbc_hookIDs[2];
char stop;

#ifdef LAB3
unsigned int sysinbcount=0;
int sys_inb_cnt(port_t port, unsigned long* byte){
	sysinbcount++;
	return sys_inb(port, byte);
}
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif


int kbd_subscribe_int(void ) {
	kbc_hookIDs[0]=0; // we'll be using id 0 for the kbc.

	int temp=kbc_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(KBC_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	kbc_hookIDs[1]=temp;

	return ret;
}

int kbd_unsubscribe_int() {
#ifdef LAB3
	printf("sys_inb was called %d times.\n", sysinbcount);
#endif
	return sys_irqrmpolicy(&kbc_hookIDs[1]);
}

void kbd_int_handler() {
	unsigned char rd;
	sys_inb_cnt(0x60, (long unsigned int*)&rd);

	if( ((rd>>7)&1) == 1){
		printf("breakcode: 0x%02x\n", rd);
	}else{
		printf(" makecode: 0x%02x\n", rd);
	}

	if(rd==0x81)
		stop=1;
	return;
}

int kbd_test_scan(){
	int ret = kbd_subscribe_int();
	if(ret!=0){
		fprintf(stderr, "Could not subscribe interruptions for the kbc!\n");exit(-1);
	}

	int ipc_status;
	int r;
	message msg;
	stop=0;
	while(!stop) { /*	You may want to use a different condition*/
		/*Get a request message.*/
		if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			int irq_set=0; irq_set |= BIT(0); // 0 as in the kbc_hookIDs[0].
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					kbd_int_handler();
				}
				break;
			default:
				break;
				//no other notifications expected: do nothing
			}
		} else { /*received a standard message, not a notification*/
			/*no standard messages expected: do nothing*/
		}
	}

	if(kbd_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_0.\n");
		exit(-2);
	}
	printf("\nunsubscribed KBD successfully.\n");

	return 0;

}

// VIDEO FUNCTIONS //

char* video_m;


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


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	int xi, yi, xf, yf;
	xi = x - size/2 + 1;
	xf = x + size/2;
	yf = y + size/2;

	video_start();


	static video_info_t vi;
	vi = get_vi();


	while(xi < xf) {
		yi = y - size/2 + 1;
		while(yi < yf) {
			setP(vi.x/2+xi, vi.y/2+yi, color);
			yi++;
		}
		xi++;
	}

	video_dump_fb();
	kbd_test_scan();

	vg_exit();

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	struct timeval  tv1, tv2;

	if (color > 63){
		color=1;
	}

	/* Coordinates arguments are checked further below */

	video_start();
	static video_info_t vi;
	vi = get_vi();

	//color start pixel
	setP(xi, yi, color);

	//color end pixel
	setP(xf, yf, color);

	// now the intermediate pixels...
	float xDir=(xf-xi)/2;
	float yDir=(yf-yi)/2;

	unsigned int dx=abs(xf-xi);
	unsigned int dy=abs(yf-yi);

	if( (xi==0 && xf==0)
			|| (yi==0 && yf==0)){
		printf("Refusing to draw on the near null edge of screen.");
		vg_exit();
	}


	/* Calculate drawing directions */
	if(dy<dx){
		yDir/=xDir;
		if(xf>xi){
			xDir=1;
		}else{
			xDir=-1;
			yDir*=-1;
		}
	}else{
		xDir/=yDir;
		if(yf>yi){
			yDir=1;
		}else{
			yDir=-1;
			xDir*=-1;
		}
	}


	/* Begin drawing */
	double tx=(double) xi, ty = (double) yi;
	while(1){
		tx+=xDir;
		ty+=yDir;

		//before writing, we check if we're inside our boundaries.
		if( (unsigned long) tx > vi.x || (unsigned long) ty > vi.y){
			printf("refusing to write out of memory!\n");
			break; // out of boundaries means we're done painting.
		}
		setP( (unsigned long) tx, (unsigned long) ty, color);

		if(dy<dx){ // use x as step variable because it changes more, thus giving us more resolution.
			if(xDir>0){ // if x is growing, we stop after we pass the target.
				if((unsigned long) tx > xf)
					break;
			}else{		// if x is shrinking, we stop after we pass the target. Notice the target is lower (i.e., xf<xi)
				if((unsigned long) tx < xf)
					break;
			}
		}else{ // use y as step variable because it changes more, thus giving us more resolution.
			if(yDir>0){ // if y is growing, we stop after we pass the target.
				if((unsigned long) ty > yf)
					break;
			}else{		// if y is shrinking, we stop after we pass the target. Notice the target is lower (i.e., yf<yi)
				if((unsigned long) ty < yf)
					break;
			}
		}
	}

	video_dump_fb();
	kbd_test_scan();
	vg_exit();

	return 0;
}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {
	// change video mode to 0x105
	video_start();

	// print xpm here
	int width, height;

	char* pix = read_xpm(xpm, &width, &height);
	width += xi;
	height += yi;
	int initial_x = xi;
	int xpm_counter = 0;

	// printing
	while(yi < height) {
		xi = initial_x;
		while(xi < width) {
			setP(xi, yi, pix[xpm_counter]);
			xi++;
			xpm_counter++;
		}
		yi++;
	}

	video_dump_fb();

	// ESC key scan
	kbd_test_scan();

	free(pix);
	// exit video mode
	vg_exit();

	return 0;
}

int test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {
	// change video mode to 0x105
	video_start();

	// fetch the sprite
	int width, height;
	char* pix = read_xpm(xpm, &width, &height);

	width += xi;
	height += yi;
	int initial_x = xi;
	int initial_y = yi;
	int xpm_counter = 0;

	// movement setup
	int movement = 0;

	if(yf > yi && xf == xi)
		movement = 0;
	else if(yf < yi && xf == xi)
		movement = 1;
	else if(yf == yi && xf > xi)
		movement = 2;
	else if(yf == yi && xf < xi)
		movement = 3;
	else {
		printf("Invalid Coordinates\n");
		return -9;
	}

	float framerate_delay = 1000000 / f;

	// printing
	switch(movement) {

	// move down
	case 0:
		while(initial_y <= yf) {
			fill_screen(0);
			while(yi < height) {
				xi = initial_x;
				while(xi < width) {
					setP(xi, yi, pix[xpm_counter]);
					xi++;
					xpm_counter++;
					video_dump_fb();
				}
				yi++;
			}

			initial_y++;
			yi = initial_y;
			height++;
			xpm_counter = 0;
			video_dump_fb();
			if(s>0) usleep(framerate_delay / s);
			else usleep(framerate_delay * s * (-1));
		}

		break;

		// move up
	case 1:
		while(initial_y >= yf) {
			fill_screen(0);
			while(yi < height) {
				xi = initial_x;
				while(xi < width) {
					setP(xi, yi, pix[xpm_counter]);
					xi++;
					xpm_counter++;
					video_dump_fb();
				}
				yi++;
			}

			initial_y--;
			yi = initial_y;
			height--;
			xpm_counter = 0;
			video_dump_fb();
			if(s>0) usleep(framerate_delay / s);
			else usleep(framerate_delay * s * (-1));
		}

		break;

		// move right
	case 2:
		while(initial_x <= xf) {
			fill_screen(0);
			while(yi < height) {
				xi = initial_x;
				while(xi < width) {
					setP(xi, yi, pix[xpm_counter]);
					xi++;
					xpm_counter++;
					video_dump_fb();
				}
				yi++;
			}

			initial_x++;
			yi = initial_y;
			width++;
			xpm_counter = 0;
			video_dump_fb();
			if(s>0) usleep(framerate_delay / s);
			else usleep(framerate_delay * s * (-1));
		}

		break;

		// move left
	case 3:
		while(initial_x >= xf) {
			fill_screen(0);
			while(yi < height) {
				xi = initial_x;
				while(xi < width) {
					setP(xi, yi, pix[xpm_counter]);
					xi++;
					xpm_counter++;
					video_dump_fb();
				}
				yi++;
			}

			initial_x--;
			yi = initial_y;
			width--;
			xpm_counter = 0;
			video_dump_fb();
			if(s>0) usleep(framerate_delay / s);
			else usleep(framerate_delay * s * (-1));
		}

		break;

	}

	// ESC key scan
	kbd_test_scan();

	free(pix);
	// exit video mode
	vg_exit();

	return 0;
}	

int test_controller() {

	static char inited=0;

	void* lm_begin;

	if(!inited){
		lm_begin = lm_init();
		inited=1;
	}

	mmap_t map; // a variable that maps the physical addr, the virtual addr, and the size of the alloc'ed memory.

	lm_alloc(512, &map);

	phys_bytes buf=map.phys;
	struct reg86u r;
	int i;

	r.u.w.ax = 0x4F00; /* VBE get mode info */
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf); /* set a segment base */
	r.u.w.di = PB2OFF(buf); /* set the offset accordingly */
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) { /* call BIOS */
		fprintf(stderr, "Couldn't call sys_int86!\n");
		return -5;
	}

	vbe_ctrl_info_t* ci = (vbe_ctrl_info_t*)map.virtual;
	printf("%d.%d\n", (ci->VbeVersion&0xff00)>>8, ci->VbeVersion&0x00ff);


	void* videoModes=lm_begin+((ci->VideoModePtr>>16) & 0xffff)*0x10+(ci->VideoModePtr&0xffff);


	for(i=0;i<40;i++){
		if(*( (uint16_t*) videoModes+i)==0xffff)
			break;
		printf("%x:", *( (uint16_t*) videoModes+i));

	}
	printf("\n%d", ci->TotalMemory);
	lm_free(&map);
	exit(1);

	return 0;
}	
