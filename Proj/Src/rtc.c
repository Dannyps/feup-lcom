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

int rtc_subscribe_int() {
	rtc_hookIDs[0]=4;

	int temp=kbc_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(RTC_IRQ_LINE, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	rtc_hookIDs[1]=temp;

	return ret;
}

int rtc_unsubscribe_int() {
	return sys_irqrmpolicy(&rtc_hookIDs[1]);
}

void wait_for_valid_rtc() {
	unsigned long reg_a = 0;

	do {
		sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_A);
		sys_inb(RTC_DATA_REG, &reg_a);
	} while (reg_a & RTC_UIP);

}

struct rtc_handler_t* rtc_int_handler() {

	unsigned long hour, min, sec, day, month, year;
	struct rtc_handler_t* rtc = NULL;

	wait_for_valid_rtc();
	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_HOURS);
	sys_inb(RTC_DATA_REG, &hour);

	wait_for_valid_rtc();
	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_MINUTES);
	sys_inb(RTC_DATA_REG, &min);

	wait_for_valid_rtc();
	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_SECONDS);
	sys_inb(RTC_DATA_REG, &sec);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_DAY_MONTH);
	sys_inb(RTC_DATA_REG, &day);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_MONTH);
	sys_inb(RTC_DATA_REG, &month);

	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_YEAR);
	sys_inb(RTC_DATA_REG, &year);

	// Set for 24hour day and BCD
	unsigned long reg_b=0;
	do {
		sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_B);
		sys_inb(RTC_DATA_REG, &reg_b);

	} while( !(reg_b & BIT(1)) && (reg_b & BIT(2)) );
	// BIT1 must be 1 for 24hourday, BIT2 must be 0 for BCD

	rtc = malloc(sizeof(struct rtc_handler_t));
	if(rtc==NULL){
		printf("Could not allocate memory\n");
		exit(-6);
	}

	rtc->hour = hour;
	rtc->hour = min;
	rtc->hour = sec;
	rtc->hour = day;
	rtc->hour = month;
	rtc->hour = year;

	return rtc;
}
