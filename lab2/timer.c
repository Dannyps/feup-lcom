#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int timer_set_frequency(unsigned char timer, unsigned long freq) {

	sys_outb(TIMER_CTRL,timer);



	return 1;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned char timer, unsigned char *st) {
	
	unsigned long oByte;

	oByte = 0 | BIT(7) | BIT(6);

	switch (timer){
		case 0:
			oByte|=BIT(1);
		break;
		case 1:
			oByte|=BIT(2);
		break;
		case 2:
			oByte|=BIT(3);
		break;

	}

	// oByte is set

	sys_outb(TIMER_CTRL, oByte);

	sys_inb(TIMER_0, (unsigned long*) &timer);

	printf("We read: %x\n", timer);

	//timer = timer | TIMER_SQR_WAVE | TIMER_BCD;

	//timer_set_frequency(timer, TIMER_FREQ);

	return 1;
}

int timer_display_conf(unsigned char conf) {
	
	return 1;
}

int timer_test_time_base(unsigned long freq) {
	
	return 1;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned char timer) {
	unsigned char st;
	// timer = bit 7,6 do control word

	timer_get_conf(timer, &st);

	timer_display_conf(st);
	
	return 1;
}
