#ifndef __RTC_H
#define __RTC_H

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71


#define RTC_IRQ_LINE 8

#define RTC_SECONDS 		0
#define RTC_MINUTES 		2
#define RTC_HOURS 			4
#define RTC_DAYOFTHEWEEK 	6
#define RTC_DAYOFTHEMONTH  	7
#define RTC_MONTH			8
#define RTC_YEAR			9

#define RTC_REGISTER_A		10
#define RTC_REGISTER_B		11
#define RTC_REGISTER_C		12
#define RTC_REGISTER_D		13


#define BIT(n) (0x01<<(n))

#endif
