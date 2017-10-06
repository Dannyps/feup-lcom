#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"


const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}


/***************   Functions   *****************/

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

	oByte = 0 | BIT(7) | BIT(6) | BIT(5);

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
	*st=timer;
	return 1;
}

int timer_display_conf(unsigned char conf) {
	printf("We read: %s\n", byte_to_binary(conf));
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
