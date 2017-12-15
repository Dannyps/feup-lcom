#include <minix/syslib.h>
#include <minix/drivers.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "timer.h"
#include "i8042.h"
#include "i8254.h"
#include "video.h"
#include "utils.h"

extern int timer0_hookIDs[2];


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
	static int tick_elapsed=0;
        if((++tick_elapsed)%1==0){
        	rfill_screen();
        	draw_main_page();
        	//printf("reprinting on %d\n", tick_elapsed);
        	vg_flush();
        }
        return;
}
