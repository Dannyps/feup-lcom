#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"

char canIWrite(){
	unsigned long ret;
	sys_inb(STATUS_REG, &ret);

	if( (ret & BIT(1)) !=0){
		return 0;
	}else{
		return 1;
	}
}

char disableMouseInterrupts(){
	if(!canIWrite()){
		printf("ERROR: KBC IN_BUF was full when attempting to request Command Byte.");
		exit(-6);
	}
	sys_outb(STATUS_REG, READ_CMD_BYTE);
	tickdelay(micros_to_ticks(DELAY_US)); // don't try to read right away
	unsigned long ret;
	sys_inb(OUT_BUF, &ret);
	printf("read %x.\n", ret);

	unsigned long mask= ~(1<<1);

	ret&=mask;

	sys_outb(CTRL_REG, WRITE_CMD_BYTE);
	sys_outb(IN_BUF, ret);  // argument

	printf("wrote %x.\n", ret);

	return 0;
}

char enableMouseInterrupts(){
	if(!canIWrite()){
		printf("ERROR: KBC IN_BUF was full when attempting to request Command Byte.");
		exit(-6);
	}
	sys_outb(STATUS_REG, READ_CMD_BYTE);
	tickdelay(micros_to_ticks(DELAY_US)); // don't try to read right away
	unsigned long ret;
	sys_inb(OUT_BUF, &ret);
	printf("read %x.\n", ret);

	unsigned long mask= (1<<1);

	ret&=mask;

	sys_outb(CTRL_REG, WRITE_CMD_BYTE);
	ret=0x47;
	sys_outb(IN_BUF, ret);

	printf("wrote %x.\n", ret);

	return 0;
}


int kbd_mouse_subscribe_int(void ) {
	lhid=2; // we'll be using id 2 for the kbc.

	int ret=sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &lhid);

	khid=lhid;
	lhid=0;

	printf("Wrting 2 mouse\n");
	wrt2Mouse(ENABLE_STREAM_MODE, 1);
	return ret;
}

int kbd_mouse_unsubscribe_int() {
	wrt2Mouse(DISABLE_STREAM_MODE, 1);
	return sys_irqrmpolicy(&khid);
}

void handleMouse(unsigned char* arr, struct mouse_action_t* ma){
	/* arr should be aligned! */
	ma->X=(unsigned int)arr[1];
	ma->Y=(unsigned int)arr[2];
	if((arr[0]&1<<4)!=0){
		// X sign bit was set.
		ma->X|=0xffffff00;
	}
	if((arr[0]&1<<5)!=0){
		// Y sign bit was set.
		ma->Y|=0xffffff00;
	}

	ma->lmb=0; ma->rmb=0; ma->mmb=0; ma->xov=0; ma->yov=0;

	// test for Middle Mouse Button
	if((arr[0]&1<<2)!=0){
		ma->mmb=1;
	}
	// test for Right Mouse Button
	if((arr[0]&1<<1)!=0){
		ma->rmb=1;
	}
	// test for Left Mouse Button
	if((arr[0]&1<<0)!=0){
		ma->lmb=1;
	}
	// test for X overflow
	if((arr[0]&1<<6)!=0){
		ma->xov=1;
	}
	// test for Y overflow
	if((arr[0]&1<<7)!=0){
		ma->yov=1;
	}
}


struct mouse_action_t* kbd_mouse_int_handler(int gesture) {
	static unsigned char count=0;
	static unsigned char arr[3];
	static char synced=0;
	static char c=0;

	ticks_elapsed = 0;
	seconds_elapsed = 0;

	// keep this value
	unsigned long rd;
	sys_inb(OUT_BUF, &rd);
	arr[count]=(unsigned char) rd;

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

	struct mouse_action_t* ma = malloc(sizeof(struct mouse_action_t));
	if(ma==NULL){
		printf("Could not allocate memory\n");
		exit(-6);
	}

	if(count>2){
		count=0;
		if(synced){
			if(!gesture){
				handleMouse(arr, &(*ma));
			} /*else{
				stateMachine(arr);
			}*/
		}
	}
	c++;

	return ma;
}

unsigned char wrt2Mouse(unsigned char cmd, char retransmit){
	unsigned char debug=1;

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

	//	tickdelay(micros_to_ticks(DELAY_US));

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
