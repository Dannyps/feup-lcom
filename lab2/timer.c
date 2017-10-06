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
  unsigned char mask;

    /* Bit 7 ** Output */
    mask=0|BIT(7);
    if(conf & mask){
      printf("Output line is ON\n");
    }else{
      printf("Output line is OFF\n");
    }

    /* Bit 6 ** Null Count */
    mask=0|BIT(6);
    if(conf & mask){
      printf("Null count is ON\n");
    }else{
      printf("Null count is OFF\n");
    }

    /* Bit 5 ** Count info */
    mask=0|BIT(5);
    if(conf & mask){
      printf("Count bit is OFF\n");
    }else{
      printf("Count bit is ON\n");
    }

    /* Bit 4 ** Status info */
    mask=0|BIT(4);
    if(conf & mask){
      printf("Status bit is OFF\n");
    }else{
      printf("Status bit is ON\n");
    }

    /* Bits 3, 2 & 1 ** timer mode */
    int mode;


    mask=0;
    if(conf & mask)
      mode=0;

    mask=0|BIT(1);
    if(conf & mask)
      mode=1;

    mask=0|BIT(2);
    if(conf & mask)
      mode=2;

    mask=0|BIT(2)|BIT(1);
    if(conf & mask)
      mode=3;

    mask=0|BIT(3);
    if(conf & mask)
      mode=4;

    mask=0|BIT(3)|BIT(1);
    if(conf & mask)
      mode=5;
    printf("Mode set is: %d.\n", mode);

	return 1;
}

int timer_set_frequency(unsigned char timer, unsigned long freq) {

	sys_outb(TIMER_CTRL, timer);

	//unsigned long clock = 1193181 / freq;

	sys_outb(TIMER_0, freq);

	return 1;
}

int timer_test_time_base(unsigned long freq) {
	
	unsigned char conf;

	// reads configuration of Timer 0
	timer_get_conf(0, &conf);

	conf = conf | BIT(4) | BIT(5);

	//test
	timer_display_conf(conf);

	timer_set_frequency(conf, freq);

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
