#include "test6.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <minix/drivers.h>
#include <minix/com.h>

static int proc_args(int argc, char **argv);
static void print_usage(char **argv);


void exit_handler(){
	printf("--- FINISHED\n");
	return;
}

int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup();

    /* Enable IO-sensitive operations for ourselves */
    //sys_iopenable(SELF);

	atexit(exit_handler);
	printf("\n--- STARTED\n");

	fflush(stdout);

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
		"\t service run %s -args \"conf\"\n"
		"\t service run %s -args \"date\"\n"
		"\t service run %s -args \"settime\"\n"
		"\t service run %s -args \"int\"\n",
		argv[0], argv[0], argv[0], argv[0]);
}


static int proc_args(int argc, char **argv)
{
	if (strncmp(argv[1], "conf", strlen("conf")) == 0) {
		if (argc != 2) {
			printf("rtc: wrong no. of arguments for rtc_test_conf()\n");
			return 1;
		}
		printf("rtc::rtc_test_conf()\n");
		return rtc_test_conf();
	}
	else if (strncmp(argv[1], "date", strlen("date")) == 0) {
		if (argc != 2) {
			printf("rtc: wrong no. of arguments for rtc_test_date()\n");
			return 1;
		}
		printf("rtc::rtc_test_date()\n");
		return rtc_test_date();
	}

	else if (strncmp(argv[1], "settime", strlen("settime")) == 0) {
		if (argc != 4) {
			printf("rtc: wrong no. of arguments for rtc_test_settime()\n");
			return 1;
		}
		printf("rtc::rtc_test_settime(%s, %s)\n", argv[2], argv[3]);
		return rtc_test_settime(argv[2], argv[3]);
	}
	else if (strncmp(argv[1], "int", strlen("int")) == 0) {
		if (argc != 2) {
			printf("rtc: wrong no. of arguments for rtc_test_int()\n");
			return 1;
		}
		printf("rtc::rtc_test_int()\n");
		return rtc_test_int();
	}
	else {
		printf("rtc: %s - no valid function!\n", argv[1]);
		return 1;
	}
	return 0;
}
