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


void printMouse(unsigned char* arr){
	/* arr should be aligned! */

	unsigned int X=(unsigned int)arr[1];
	unsigned int Y=(unsigned int)arr[2];

	if((arr[0]&1<<4)!=0){
		// X sign bit was set.
		X|=0xffffff00;
	}

	if((arr[0]&1<<5)!=0){
		// Y sign bit was set.
		Y|=0xffffff00;
	}

	unsigned char lmb=0, rmb=0, mmb=0, xov=0, yov=0;

	// test for Middle Mouse Button
	if((arr[0]&1<<2)!=0){
		mmb=1;
	}

	// test for Right Mouse Button
	if((arr[0]&1<<1)!=0){
		rmb=1;
	}

	// test for Left Mouse Button
	if((arr[0]&1<<0)!=0){
		lmb=1;
	}

	// test for X overflow
	if((arr[0]&1<<6)!=0){
		xov=1;
	}

	// test for Y overflow
	if((arr[0]&1<<7)!=0){
		yov=1;
	}


	printf("B1=0x%x	B2=0x%x	B3=0x%x	LB=%d	MB=%d	RB=%d	XOV=%d	YOV=%d	X=%d	Y=%d\n", arr[0], arr[1], arr[2], lmb, mmb, rmb, xov, yov, X, Y);
}

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
	static unsigned char count=0;
	static unsigned char arr[3];
	static char synced=0;
	static char c=0;


	// keep this value
	unsigned long rd;
	sys_inb(OUT_BUF, &rd);
	arr[count]=(unsigned char) rd;


	//printf("read %x\n", rd);

	// Update counter
	count++;
	if(!synced && c>3){
		//try to sync
			if( (arr[count-1] & (1<<3) ) == 0){ // we got a zero on bit 3. This byte wasn't the first, but the next one might be it.
				count=0;
			}else{
				// we got a one.
				if(count-1==0 &&
					( (arr[1] & (1<<3)) == 0) &&
					( (arr[2] & (1<<3)) == 0) ){
					//that last byte was byte 0, and the following two bytes were good. We're synced.
					synced=1;
					//printf("SYNCED!\n");
				}
			}
	}


	if(count>2){
		count=0;
		if(synced) printMouse(arr);
	}
	c++;
	return;
}

int mouse_test_packet(unsigned short cnt){
   	printf("reading %lu packets before exiting...\n", cnt);
	cnt*=3; // each printed packet has 3 packets.
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

int mouse_test_remote(unsigned long period, unsigned short cnt){
   	printf("reading %lu packets before exiting...\n", cnt);
	//cnt*=3; // each printed packet has 3 packets.
	period*=1000;

	sys_outb(STATUS_REG, 0x20);

	unsigned long ret;
	sys_inb(OUT_BUF, &ret);

	printf("%8x\n", ret);

	exit(0);
	//kbd_subscribe_int();

	wrt2Mouse(DISABLE_STREAM_MODE, 1);
	wrt2Mouse(SET_REMOTE_MODE, 1);

	int i, j;
	for(i=0;i<cnt;i++){
		wrt2Mouse(READ_DATA, 1);
		unsigned char arr[3];
		for(j=0;j<3;j++){
			//tickdelay(micros_to_ticks(DELAY_US)); // don't try to read right away
			unsigned long rd;
			sys_inb(OUT_BUF, &rd);
			arr[j]=(unsigned char) rd;
		}
		printMouse(arr);
		tickdelay(micros_to_ticks(period));
	}

	//kbd_unsubscribe_int();
	return 0;
}	

int mouse_test_async(unsigned short idle_time){
	return 0;
}	


int mouse_test_gesture(short length){
	return 0;
}	
