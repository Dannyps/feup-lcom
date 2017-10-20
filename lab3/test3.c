#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"

#ifdef LAB3
unsigned int sysinbcount;
int sys_inb_cnt(port_t port, unsigned long* byte){
	return sys_inb(port, byte);
}
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

/*  GLOBAL VARIABLES  */

int lhid, khid;
char stop;


/*     FUNCTIONS      */

int kbd_subscribe_int(void ) {
	lhid=0; // we'll be using id 0 for the kbc.

	int ret=sys_irqsetpolicy(KBC_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &lhid);

	khid=lhid;
	lhid=0;
	return ret;
}

int kbd_unsubscribe_int() {
	#ifdef LAB3
		printf("sys_inb was called %d times.\n", sysinbcount);
	#endif
	return sys_irqrmpolicy(&khid);
}

void kbd_int_handler() {
	//static unsigned char count=0;

	//printf("received an interrupt!");

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


int kbd_test_scan_c(){
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
			 int irq_set=0; irq_set |= BIT(0); // 0 as in the lhid.
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
		 //if(time_elapsed>=time){break;}
	}

	if(kbd_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_0.\n");
		exit(-9);
	}
	printf("\nunsubscribed successfully.\n");

	return 0;

}

int kbd_test_scan(unsigned short assembly) {
	if(assembly==0)
		return kbd_test_scan_c();
	/* To be completed */
	return 0;
}
int kbd_test_poll() {
    /* To be completed */
	return 0;
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
	return 0;
}
