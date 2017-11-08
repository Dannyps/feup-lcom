#include <stdio.h>
#include "i8042.h"
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

#define DELAY_US    20000


int lhid, khid;
//unsigned long packets2read=0;

unsigned char wrt2Mouse(unsigned char cmd, char retransmit){
	uchar debug=1;

	if(debug){
		printf("Writing WRITE BYTE command to CTRL_REG... ");
	}

	if(sys_outb(CTRL_REG, 0xD4)==0 && debug){
		printf("done.\n");
	}


	if(debug){
		printf("Writing command to IN_BUF... ");
	}

	if(sys_outb(IN_BUF, cmd)==0 && debug){
		printf("done.\n");
	}

	tickdelay(micros_to_ticks(DELAY_US));

	unsigned long ret;
	sys_inb(OUT_BUF, &ret);

	if( ret == MOUSE_NACK){
		if(retransmit){
			// let's be optimistic and try this again, shall we?
			wrt2Mouse(cmd, 1);
		}else{
			return -1;
		}
	}else if(ret == MOUSE_ERROR){
		// it's the second consecutive error we get. FIA.
		return -2;
	} // else => success

	return 0;
}

int kbd_subscribe_int(void ) {
	lhid=0; // we'll be using id 0 for the kbc.

	int ret=sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &lhid);

	khid=lhid;
	lhid=0;

	wrt2Mouse(ENABLE_STREAM_MODE, 1);
	return ret;
}

int kbd_unsubscribe_int() {
	wrt2Mouse(DISABLE_STREAM_MODE, 1);
	return sys_irqrmpolicy(&khid);
}

void kbd_mouse_int_handler() {
	printf("entered!\n");
	unsigned char rd;
	sys_inb(0x60, (long unsigned int*)&rd);

	printf("read %x\n", rd);
	return;
}

int mouse_test_packet(unsigned short cnt){
   	printf("reading %lu packets before exiting...\n", cnt);
   	//packets2read=cnt;
	int ret = kbd_subscribe_int();
	if(ret!=0){
		fprintf(stderr, "Could not subscribe interruptions for the kbc!\n");exit(-1);
	}

	int ipc_status;
	int r;
	message msg;
	while(cnt>0) { /*	You may want to use a different condition*/
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
						 cnt--;
						 kbd_mouse_int_handler();
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

int mouse_test_async(unsigned short idle_time){
	return 0;
}	

int mouse_test_remote(unsigned long period, unsigned short cnt){
	return 0;
}	

int mouse_test_gesture(short length){
	return 0;
}	
