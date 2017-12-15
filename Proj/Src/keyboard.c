#include <stdio.h>
#include <stdlib.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "i8254.h"
#include <minix/sysutil.h>
#include "keyboard.h"

// KEYBOARD FUCNTIONS //

extern int kbc_hookIDs[2];

int kbd_subscribe_int(void ) {
	kbc_hookIDs[0]=0; // we'll be using id 0 for the kbc.

	int temp=kbc_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(KBC_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	kbc_hookIDs[1]=temp;

	return ret;
}

int kbd_unsubscribe_int() {
	return sys_irqrmpolicy(&kbc_hookIDs[1]);
}

struct key_press_t* kbd_int_handler() {
	struct key_press_t* kp = malloc(sizeof(struct key_press_t));
	//printf("Alloc'ing %d bytes of memory.\n", sizeof(struct key_press_t));
	if(kp==NULL){
		printf("Could not allocate memory\n");
		exit(-6);
	}
	long unsigned int rd;
	static char flag=0;

	sys_inb(0x60, &rd);

	if(rd == 0xe0){
		printf("0x0e... ");
		flag=1;
		return NULL;
	}else if( ((rd>>7)&1) == 1){
		printf("breakcode: 0x%02x\n", rd);
	}else{
		printf(" makecode: 0x%02x\n", rd);
	}

	kp->is2Byte = flag;
	flag=0; // for next iter;
	kp->code=(unsigned char) rd;
	return kp;
}
