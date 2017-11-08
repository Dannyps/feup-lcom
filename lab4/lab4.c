#include "test4.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/drivers.h>


static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);


void exit_handler(){
	printf("--- FINISHED\n");

	return;
}

int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup();
	atexit(exit_handler); // install exit handler
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
			"\t service run %s -args \"packet\"\n"
			"\t service run %s -args \"remote\"\n"
			"\t service run %s -args \"async\"\n"
			"\t service run %s -args \"gesture\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}


static int proc_args(int argc, char **argv)
{
	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("kbd_m: wrong number of arguments for mouse_test_packet()\n");
			return 1;
		}

		unsigned long cnt = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (cnt == ULONG_MAX)
			return 1;
		printf("kbd_m::mouse_test_packet(%lu)\n", cnt);
		return mouse_test_packet(cnt);
	}
//	else if (strncmp(argv[1], "poll", strlen("poll")) == 0) {
//		if (argc != 2) {
//			printf("kbd: too many arguments for kbd_test_poll()\n");
//			return 1;
//		}
//		printf("--- NOTE: remember to deactivate Minix's IH.\nkbd::kbd_test_poll()\n");
//		return kbd_test_poll();
//	}
//	else if (strncmp(argv[1], "tscan", strlen("tscan")) == 0) {
//		if (argc != 3) {
//			printf("kbd: wrong number of arguments for kbd_test_timed_scan()\n");
//			return 1;
//		}
//		time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
//		if (time == ULONG_MAX)
//			return 1;
//		printf("kbd::kbd_test_timed_scan(%lu)\n", time);
//		return kbd_test_timed_scan(time);
//	}
	else {
		printf("kbd_m: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}


