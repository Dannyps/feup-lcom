#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"

// KEYBOARD FUCNTIONS //


int kbd_subscribe_int(void ) {
	kbc_hookIDs[0]=0; // we'll be using id 0 for the kbc.

	int temp=kbc_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(KBC_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	kbc_hookIDs[1]=temp;

	return ret;
}

int kbd_unsubscribe_int() {
#ifdef PROJ
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
	while(!stop) {
		//Get a request message.
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
		} else { // received a standard message, not a notification
			// no standard messages expected: do nothing
		}
	}

	if(kbd_unsubscribe_int()!=0){
		fprintf(stderr, "Could not unsubscribe from IRQ_0.\n");
		exit(-2);
	}
	printf("\nunsubscribed KBD successfully.\n");

	return 0;

}

