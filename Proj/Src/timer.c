#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "timer.h"

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
	if((++ticks_elapsed)%60==0){
		seconds_elapsed++;
		printf("%d seconds have gone by\n", seconds_elapsed);
	}

	return;
}
