#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include "utils.h"
#include "video_gr.h"
#include "view.h"

extern View cal;

int mouseInBox(unsigned tlX, unsigned tlY, unsigned brX, unsigned brY){

	return cursorX>tlX && cursorX<brX && cursorY>tlY && cursorY<brY;
}

void check_clicks(struct mouse_action_t* ma){
	if(search){
		if(mouseInBox(812, 228, 862, 278) && ma->lmb){ // exit box
			search=0;
		}
		if(mouseInBox(698, 427, 748, 463) && ma->lmb){ // ok box
			search=interpretSrchStr();
		}
		if(mouseInBox(755, 427, 853, 463) && ma->lmb){ // reset field box
			srchStr[0]='\0';
		}
	}else{
		if(mouseInBox(650, 20, 700, 70) && ma->lmb){ // exit box
			stop=1;
		}

		if(mouseInBox(335, 516, 393, 574) && ma->lmb){ // arrow left month
			prevMonth(&cal);
		}

		if(mouseInBox(600, 516, 658, 574) && ma->lmb){ // arrow right month
			nextMonth(&cal);
		}

		if(mouseInBox(335, 625, 412, 655) && ma->lmb){ // arrow left year
			prevYear(&cal);
		}

		if(mouseInBox(574, 625, 654, 655) && ma->lmb){ // arrow right year
			nextYear(&cal);
		}
		if(mouseInBox(425, 570, 574, 620) && ma->lmb){ // search box
			search=1;
		}
	}


}

void calc_new_mouse_coords(struct mouse_action_t* ma){
	video_info_t vi=get_vi();

//	if(cursorX+ma->x+5>vi.x){
//		return;
//	}
//	if(cursorY-(ma->y)>vi.y){
//		return;
//	}
//
//	if( (((int) cursorX) - (int) ma->x) < 0){
//		return;
//	}
//	if( (((int) cursorY) + (int) ma->y) < 0){
//		return;
//	}
	cursorX+=ma->x;
	cursorY-=ma->y;
	cursorX=max(cursorX, 0);
	cursorX=min(cursorX, vi.x-5);

	cursorY=max(cursorY, 0);
	cursorY=min(cursorY, vi.y-5);

//	if(cursorX<0)
//		cursorX=0+1;
//	if(cursorY<0)
//		cursorY=0+1;
//	if(cursorX>vi.x)
//		cursorX=vi.x-1;
//	if(cursorY>vi.y)
//		cursorY=vi.y-1;
}

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
	//clear stuck byte
	unsigned long rd;
	sys_inb(OUT_BUF, &rd);

	if(!canIWrite()){
		printf("ERROR: KBC IN_BUF was full when attempting to request Command Byte.");
		exit(-6);
	}

	mouse_hookIDs[0]=2;
	int temp=mouse_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(MOUSE_IRQ, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	mouse_hookIDs[1]=temp;

	printf("Wrting 2 mouse\n");
	wrt2Mouse(ENABLE_STREAM_MODE, 1);

	// Activate mouse wheel
	unsigned long mouseReturn;

	wrt2Mouse(0xf3, 1); // Set sample rate...
	wrt2Mouse(200, 	0); // to 200.
	wrt2Mouse(0xf3, 1); // Set sample rate...
	wrt2Mouse(100, 	0); // to 100.
	wrt2Mouse(0xf3, 1); // Set sample rate...
	wrt2Mouse(80, 	0); // to 80.

	wrt2Mouse(0xf2, 0); // get device id

	sys_inb(OUT_BUF, &mouseReturn);


	if(mouseReturn==0x03){
		// successs! we have a special scrolling wheel capable mouse atached to our PS/2 port,
		// and we're gonna use it ;)
	}else{
		// :(  aka sad face
		printf("The current mouse does not support the Microsoft Intellimouse scrolling wheel.\nPlease use another method.\n"); exit(3);
	}
	return ret;
}

int kbd_mouse_unsubscribe_int() {
	wrt2Mouse(DISABLE_STREAM_MODE, 1);
	return sys_irqrmpolicy(&mouse_hookIDs[1]);
}

void handleMouse(unsigned char* arr, struct mouse_action_t* ma){
	/* arr should be aligned! */
	ma->x=(unsigned char)arr[1];
	ma->y=(unsigned char)arr[2];
	ma->z=(char) arr[3];
	if((arr[0]&1<<4)!=0){
		// X sign bit was set.
		ma->x|=0xffffff00;
	}
	if((arr[0]&1<<5)!=0){
		// Y sign bit was set.
		ma->y|=0xffffff00;
	}

	ma->lmb=0; ma->rmb=0; ma->mmb=0; //ma->xov=0; ma->yov=0;

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
//	// test for X overflow
//	if((arr[0]&1<<6)!=0){
//		ma->xov=1;
//	}
//	// test for Y overflow
//	if((arr[0]&1<<7)!=0){
//		ma->yov=1;
//	}

	//take care of the mouse on screen
	calc_new_mouse_coords(ma);

	//check if a box on screen has been clicked.
	check_clicks(ma);
}


struct mouse_action_t* kbd_mouse_int_handler() {
	//printf("mouse handler... ");
	static unsigned char count=0;
	static unsigned char arr[4];
	static char synced=0;
	static char c=0;
	struct mouse_action_t* ma = NULL;

	// keep this value
	unsigned long rd;
	sys_inb(OUT_BUF, &rd);
	arr[count]=(unsigned char) rd;

	// Update counter
	count++;
	if(!synced && c>4){
		//try to sync
			if( (arr[count-1] & (1<<3) ) == 0){ // we got a zero on bit 3. This byte wasn't the first, but the next one might be it.
				count=0;
			}else{
				// we got a one.
				if(count-1==0 &&
					( (arr[1] & (1<<3)) == 0) &&
					( (arr[2] & (1<<3)) == 0) &&
					( (arr[3] & (1<<3)) == 0) ){
					//that last byte was byte 0, and the following two bytes were good. We're synced.
					synced=1;
					printf("==> MOUSE SYNCED!\n");
				}
			}
	}
	if(count>3){
		count=0;
		if(synced){
			ma = malloc(sizeof(struct mouse_action_t));
			if(ma==NULL){
				printf("Could not allocate memory\n");
				exit(-6);
			}
			handleMouse(arr, &(*ma));
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
