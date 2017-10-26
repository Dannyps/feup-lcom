#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "i8254.h"
#include <minix/sysutil.h>

#define DELAY_US    20000


#ifdef LAB3
unsigned int sysinbcount=0;
int sys_inb_cnt(port_t port, unsigned long* byte){
	sysinbcount++;
	return sys_inb(port, byte);
}
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

/*  GLOBAL VARIABLES  */

/*
 *  Hook IDs structure
 *
 * int[2];
 *  _____________________________________________
 * |		  0			  |			  1          |
 * |----------------------|----------------------|
 * | lhid (local hook id) |	khid (kernel hook id)|
 * |______________________|______________________|
 *
 * */

int timer0_hookIDs[2];
int kbc_hookIDs[2];

int seconds_elapsed=0;
int tick_elapsed=0;

char stop;


/*     FUNCTIONS      */

int kbd_subscribe_int(void ) {
	kbc_hookIDs[0]=0; // we'll be using id 0 for the kbc.

	int temp=kbc_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(KBC_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	kbc_hookIDs[1]=temp;

	return ret;
}

int timer0_subscribe_int(void ) {
	timer0_hookIDs[0]=1; // we'll be using id 1 for the timer_0.

	int temp=timer0_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(TIMER0_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	timer0_hookIDs[1]=temp;

	return ret;
}

int timer0_unsubscribe_int() {

        return sys_irqrmpolicy(&timer0_hookIDs[1]);

}

void timer0_int_handler() {

        if((++tick_elapsed)%60==0){

                printf("1 sec! %d\n", ++time_elapsed);

        }

        return;

}



int kbd_unsubscribe_int() {
	#ifdef LAB3
		printf("sys_inb was called %d times.\n", sysinbcount);
	#endif
	return sys_irqrmpolicy(&kbc_hookIDs[1]);
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
		 //if(time_elapsed>=time){break;}
	}

	if(kbd_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_0.\n");
		exit(-2);
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
    unsigned char status;
	char stop=0;
    while(!stop){
    	sys_inb_cnt(STATUS_REG, (long unsigned int*)&status);
		if((status&0x01)==0){ // the output buffer is empty. We should wait a little bit
			tickdelay(micros_to_ticks(DELAY_US));
			continue;
		}else{ // we can read!
			unsigned char rd;
			sys_inb_cnt(0x60, (long unsigned int*)&rd);

			if( ((rd>>7)&1) == 1){
				printf("breakcode: 0x%02x\n", rd);
			}else{
				printf(" makecode: 0x%02x\n", rd);
			}

			if(rd==0x81)
				stop=1;
		}
	}

	#ifdef LAB3
		printf("sys_inb was called %d times.\n", sysinbcount);
	#endif

	return 0;
}



int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
	
	/* Subscribes to timer */
	int timer0_ret = timer0_subscribe_int();
    if(timer0_ret!=0){
        fprintf(stderr, "Couldn't subscribe to IRQ_0!\n");exit(-3);
    }

	/* Subscribes to kbd */
	int ret = kbd_subscribe_int();
	if(ret!=0){
		fprintf(stderr, "Could not subscribe interruptions for the kbc!\n");exit(-4);
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
					if (msg.NOTIFY_ARG & irq_timer0) {
						timer0_int_handler();
					}
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
		if(seconds_elapsed >= n)
			{break;}
	}

	if(timer_unsubscribe_int()!=0){
        fprintf(stderr, "Could not unsubscribe from IRQ_0.\n"); exit(-5);
    }

	
	if(kbd_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_0.\n");	exit(-6);
	}
	
	printf("\nunsubscribed successfully.\n");

	return 0;
}


