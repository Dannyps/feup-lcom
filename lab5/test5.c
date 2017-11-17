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

#ifdef LAB5
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

void setP(unsigned long x, unsigned long y, unsigned char color){
	video_m[(y*H_RES+x) + (H_RES*V_RES/2 + H_RES/2)]=color;
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
	
	video_start();

	int i, j;

	for(i=0; i < size; i++) {
		for(j=0; j < size; j++)
			setP(x+i-size/2, y+j-size/2, color);
	}

	kbd_test_scan();

	vg_exit();

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
	
