#include <stdio.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "vbe.h"
#include "video_gr.c"
#include "i8042.h"
#include <minix/sysutil.h>

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
	if(color>63){
		return -1;
	}
	int i;
	for(i=0;i<V_RES*H_RES;i++){
		video_m[i]=color;
	}
	return 0;
}

void setP(unsigned long x, unsigned long y, unsigned char color){
	if(video_m==0){
		printf("Can't write pixel if the screen is not in video mode! Exiting...\n");
		exit(-7);
	}else
		video_m[y*H_RES+x]=color;

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

	while(xi < xf) {
		yi = y - size/2 + 1;
		while(yi < yf) {
			setP(H_RES/2+xi, V_RES/2+yi, color);
			yi++;
		}
		xi++;
	}

	kbd_test_scan();

	vg_exit();

	return 0;
}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {
	
	if (color > 63){
			color=1;
	}

	/* Coordinates arguments are checked further below */

	video_start();
	//color start pixel
	setP(xi, yi, 5);

	//color end pixel
	setP(xf, yf, 5);

	// now the intermediate pixels...
	float xDir=(xf-xi)/2;
	float yDir=(yf-yi)/2;

	unsigned int dx=abs(xf-xi);
	unsigned int dy=abs(yf-yi);

	if(xi==0 && xf==0 || yi==0 && yf==0){
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
		if( (unsigned long) tx <0 ||  (unsigned long) tx > H_RES || (unsigned long) ty <0 || (unsigned long) ty > V_RES){
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

	kbd_test_scan();
	vg_exit();

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
	
