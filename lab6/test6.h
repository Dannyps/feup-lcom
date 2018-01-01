int rtc_test_conf(void);
int rtc_test_date(void);
int rtc_test_int();
int rtc_test_settime(char* s1, char* s2);

typedef struct {
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char day;
	unsigned char month;
	unsigned char year;
	unsigned char wd;
} rtc_time_t;
