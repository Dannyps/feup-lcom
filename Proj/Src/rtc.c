#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "i8254.h"
#include "rtc.h"
#include "utils.h"
#include "video_gr.h"

extern int rtc_hoodIDs[2];


void clear_regC(){
	unsigned long a;
	sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_C);
	sys_inb(RTC_DATA_REG, &a);
}

void enable_update_interrupts(){
	long unsigned a;
	sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_B);
	sys_inb(RTC_DATA_REG, &a);
	printf("reg b is 0x%x\n", a);
	a &= ~BIT(5); // unset set bit 5 -- PIE (periodic)
	a &= ~BIT(6); // unset set bit 6 -- AIE (alarm)
	a |= BIT(4); // set bit 4 -- UIE (update done)
	printf("writing  0x%x... ", a);
	sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_B);
	sys_outb(RTC_DATA_REG, a);
	printf("done\n");
}

void disable_update_interrupts(){
	long unsigned a;
	sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_B);
	sys_inb(RTC_DATA_REG, &a);
	a &= ~BIT(4); // unset bit 4 -- UIE (update done)
	sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_B);
	sys_outb(RTC_DATA_REG, a);
}


int rtc_subscribe_int() {
	clear_regC();
	/* Begin by enabling UIP interrupts */
	enable_update_interrupts();


	rtc_hookIDs[0]=4;

	int temp=rtc_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(RTC_IRQ_LINE, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	rtc_hookIDs[1]=temp;

	return ret;
}

// https://stackoverflow.com/a/42340213
int bcd_to_decimal(unsigned char x) {
    return x - 6 * (x >> 4);
}

int rtc_unsubscribe_int() {
	disable_update_interrupts();
	return sys_irqrmpolicy(&rtc_hookIDs[1]);
}

void wait_for_valid_rtc() {
	unsigned long reg_a = 0;

	do {
		sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_A);
		sys_inb(RTC_DATA_REG, &reg_a);
	} while (reg_a & RTC_UIP);

}

rtc_time_t* rtc_int_handler() {
	/**
	 * Make sure you call this function on the right instant!
	 */

	long unsigned a, b, c, d, e, f, g;
	rtc_time_t* ret = malloc(sizeof(rtc_time_t));
	if(ret==NULL){
		printf("out of memory.\n");exit(-5);
	}
	// will wait for an UIP

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_SECONDS);
	sys_inb(RTC_DATA_REG, &a);
	ret->sec=bcd_to_decimal((unsigned char)a);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_MINUTES);
	sys_inb(RTC_DATA_REG, &b);
	ret->min=bcd_to_decimal((unsigned char)b);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_HOURS);
	sys_inb(RTC_DATA_REG, &c);
	ret->hour=bcd_to_decimal((unsigned char)c);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_DAY_WEEK);
	sys_inb(RTC_DATA_REG, &d);
	ret->wd=bcd_to_decimal((unsigned char)d);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_DAY_MONTH);
	sys_inb(RTC_DATA_REG, &e);
	ret->day=bcd_to_decimal((unsigned char)e);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_MONTH);
	sys_inb(RTC_DATA_REG, &f);
	ret->month=bcd_to_decimal((unsigned char)f);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_YEAR);
	sys_inb(RTC_DATA_REG, &g);
	ret->year=bcd_to_decimal((unsigned char)g);

	return ret;
}
