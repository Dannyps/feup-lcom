#ifndef __RTC_H
#define __RTC_H

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_IRQ_LINE 8

// RTC Internal Address Space
#define RTC_ADDRESS_SECONDS 		0
#define RTC_ADDRESS_SECONDS_ALARM 	1
#define RTC_ADDRESS_MINUTES			2
#define RTC_ADDRESS_MINUTES_ALARM	3
#define RTC_ADDRESS_HOURS 			4
#define RTC_ADDRESS_HOURS_ALARM 	5
#define RTC_ADDRESS_DAY_WEEK 		6
#define RTC_ADDRESS_DAY_MONTH		7
#define RTC_ADDRESS_MONTH			8
#define RTC_ADDRESS_YEAR			9
#define RTC_CTRL_REG_A 				10
#define RTC_CTRL_REG_B  			11
#define RTC_CTRL_REG_C  			12
#define RTC_CTRL_REG_D 				13

#define RTC_UIP						BIT(7)

typedef struct rtc_handler_t {
	unsigned long hour;
	unsigned long min;
	unsigned long sec;
	unsigned long day;
	unsigned long month;
	unsigned long year;
} RTC_HANDLER;

int rtc_subscribe_int();
int rtc_unsubscribe_int();
void wait_for_valid_rtc();
struct rtc_handler_t* rtc_int_handler();

#endif
