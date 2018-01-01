#include "rtc.h"
#include "test6.h"

#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void enable_update_interrupts();
void disable_update_interrupts();
void clear_regC();


int rtc_hookIDs[2];

// https://stackoverflow.com/a/42340213
int bcd_to_decimal(unsigned char x) {
    return x - 6 * (x >> 4);
}


int rtc_test_conf(void) {
	long unsigned a, b, c, d;
	// will wait for an UIP
	do{
		sys_outb(RTC_ADDR_REG, RTC_REGISTER_A);
		sys_inb(RTC_DATA_REG, &a);
		printf("got 0x%x\n", a);
	} while (!(BIT(7)&a));

	sys_outb(RTC_ADDR_REG, RTC_REGISTER_A);
	sys_inb(RTC_DATA_REG, &a);
	printf("a: 0x%x\n", a);

	sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
	sys_inb(RTC_DATA_REG, &b);
	printf("b: 0x%x\n", b);

	sys_outb(RTC_ADDR_REG, RTC_REGISTER_C);
	sys_inb(RTC_DATA_REG, &c);
	printf("c: 0x%x\n", c);

	sys_outb(RTC_ADDR_REG, RTC_REGISTER_D);
	sys_inb(RTC_DATA_REG, &d);
	printf("d: 0x%x\n", d);

	return 0;

}

int rtc_test_date(void) {
	long unsigned a, b, c, d, e, f, g;
	// will wait for an UIP

	sys_outb(RTC_ADDR_REG, RTC_SECONDS);
	sys_inb(RTC_DATA_REG, &a);
	printf("seconds	: 0x%x\n", a);

	sys_outb(RTC_ADDR_REG, RTC_MINUTES);
	sys_inb(RTC_DATA_REG, &b);
	printf("minutes	: 0x%x\n", b);

	sys_outb(RTC_ADDR_REG, RTC_HOURS);
	sys_inb(RTC_DATA_REG, &c);
	printf("hours	: 0x%x\n", c);

	sys_outb(RTC_ADDR_REG, RTC_DAYOFTHEWEEK);
	sys_inb(RTC_DATA_REG, &d);
	printf("weekDay	: 0x%x\n", d);

	sys_outb(RTC_ADDR_REG, RTC_DAYOFTHEMONTH);
	sys_inb(RTC_DATA_REG, &e);
	printf("monthDay: 0x%x\n", e);

	sys_outb(RTC_ADDR_REG, RTC_MONTH);
	sys_inb(RTC_DATA_REG, &f);
	printf("month	: 0x%x\n", f);

	sys_outb(RTC_ADDR_REG, RTC_YEAR);
	sys_inb(RTC_DATA_REG, &g);
	printf("year	: 0x%x\n", g);
	return 0;
}

rtc_time_t rtc_get_time(){
	/**
	 * Make sure you call this function on the right instant!
	 */

	long unsigned a, b, c, d, e, f, g;
	rtc_time_t ret;
	// will wait for an UIP

	sys_outb(RTC_ADDR_REG, RTC_SECONDS);
	sys_inb(RTC_DATA_REG, &a);
	ret.sec=bcd_to_decimal((unsigned char)a);

	sys_outb(RTC_ADDR_REG, RTC_MINUTES);
	sys_inb(RTC_DATA_REG, &b);
	ret.min=bcd_to_decimal((unsigned char)b);

	sys_outb(RTC_ADDR_REG, RTC_HOURS);
	sys_inb(RTC_DATA_REG, &c);
	ret.hour=bcd_to_decimal((unsigned char)c);

	sys_outb(RTC_ADDR_REG, RTC_DAYOFTHEWEEK);
	sys_inb(RTC_DATA_REG, &d);
	ret.wd=bcd_to_decimal((unsigned char)d);

	sys_outb(RTC_ADDR_REG, RTC_DAYOFTHEMONTH);
	sys_inb(RTC_DATA_REG, &e);
	ret.day=bcd_to_decimal((unsigned char)e);

	sys_outb(RTC_ADDR_REG, RTC_MONTH);
	sys_inb(RTC_DATA_REG, &f);
	ret.month=bcd_to_decimal((unsigned char)f);

	sys_outb(RTC_ADDR_REG, RTC_YEAR);
	sys_inb(RTC_DATA_REG, &g);
	ret.year=bcd_to_decimal((unsigned char)g);

	return ret;

}

int rtc_test_settime(char* s1, char* s2){
	// TODO use args instead of fixed values
	long unsigned b, nb;

	sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
	sys_inb(RTC_DATA_REG, &b);
	nb=b|BIT(7);

	/* write set bit on regB*/
	sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
	sys_outb(RTC_DATA_REG, nb);

	/* seconds */
	sys_outb(RTC_ADDR_REG, RTC_SECONDS);
	sys_outb(RTC_DATA_REG, 0x00);

	/* minutes */
	sys_outb(RTC_ADDR_REG, RTC_MINUTES);
	sys_outb(RTC_DATA_REG, 0x12);

	/* hours */
	sys_outb(RTC_ADDR_REG, RTC_HOURS);
	sys_outb(RTC_DATA_REG, 0x11);

	/* day of week */
	sys_outb(RTC_ADDR_REG, RTC_DAYOFTHEWEEK);
	sys_outb(RTC_DATA_REG, 0x02);

	/* day of month */
	sys_outb(RTC_ADDR_REG, RTC_DAYOFTHEMONTH);
	sys_outb(RTC_DATA_REG, 0x01);

	/* month */
	sys_outb(RTC_ADDR_REG, RTC_MONTH);
	sys_outb(RTC_DATA_REG, 0x01);

	/* year */
	sys_outb(RTC_ADDR_REG, RTC_YEAR);
	sys_outb(RTC_DATA_REG, 0x18);

	/* write !set bit on regB*/
	sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
	sys_outb(RTC_DATA_REG, b);

	return 0;
}

void enable_update_interrupts(){
	long unsigned a;
	sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
	sys_inb(RTC_DATA_REG, &a);
	printf("reg b is 0x%x\n", a);
	a &= ~BIT(5); // set bit 4 -- UIE (update done)
	a &= ~BIT(6); // set bit 4 -- UIE (update done)
	a |= BIT(4); // set bit 4 -- UIE (update done)
	printf("writing  0x%x... ", a);
	sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
	sys_outb(RTC_DATA_REG, a);
	printf("done\n");
}

void disable_update_interrupts(){
	long unsigned a;
	sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
	sys_inb(RTC_DATA_REG, &a);
	a &= ~BIT(4); // unset bit 4 -- UIE (update done)
	sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
	sys_outb(RTC_DATA_REG, a);
}

int rtc_subscribe_int(){
	rtc_hookIDs[0]=4;

	int temp=rtc_hookIDs[0];  // use the temp variable for input/output from sys_irqsetpolicy

	int ret=sys_irqsetpolicy(RTC_IRQ_LINE, IRQ_EXCLUSIVE|IRQ_REENABLE, &temp);

	sys_irqenable(&temp);

	rtc_hookIDs[1]=temp;

	return ret;
}

int rtc_unsubscribe_int() {
	return sys_irqrmpolicy(&rtc_hookIDs[1]);
}

int rtc_int_handler(){
	rtc_test_date();
	clear_regC();
	return 0;
}

void clear_regC(){
	unsigned long a;
	sys_outb(RTC_ADDR_REG, RTC_REGISTER_C);
	sys_inb(RTC_DATA_REG, &a);
}

int rtc_test_int() {
	clear_regC();
	/* Begin by enabling UIP interrupts */
	enable_update_interrupts();
	rtc_subscribe_int();


	int ipc_status;
	int r;
	message msg;
	printf("Entering the loop.\n");
	int i=0;
	while(i<5) { /*	You may want to use a different condition*/
		/*Get a request message.*/
		if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		printf("received!\n");
		if (is_ipc_notify(ipc_status)) {
			int irq_rtcset=0; irq_rtcset |= BIT(4) ; // 4 as in the rtc_hookIDs[0].

			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_rtcset) {
					rtc_time_t t=rtc_get_time();
					printf("seconds is %d, minutes is %d, hours is %d\n", t.sec, t.min, t.hour);
					clear_regC();
					i++;
					// Print date and time to the screen
				}

				break;
			default:
				break;
				//no other notifications expected: do nothing
			}
		} else { /*received a standard message, not a notification*/
			/*no standard messages expected: do nothing*/
		}
	}

	disable_update_interrupts();
	rtc_unsubscribe_int();
	return 0;
}
