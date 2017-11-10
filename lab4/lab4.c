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
			"\t service run %s -args \"packet <cnt>\"\n"
			"\t service run %s -args \"remote <period> <cnt>\"\n"
			"\t service run %s -args \"async <idle_time>\"\n"
			"\t service run %s -args \"gesture <length>\"\n",
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

	else if (strncmp(argv[1], "remote", strlen("remote")) == 0) {
		if (argc != 4) {
			printf("kbd_m: wrong number of arguments for mouse_test_remote()\n");
			return 1;
		}

		unsigned long period = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (period == ULONG_MAX)
			return 1;

		unsigned long cnt = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (cnt == ULONG_MAX)
			return 1;
		printf("kbd_m::mouse_test_packet(%lu, %lu)\n", period, cnt);
		return mouse_test_remote(period, cnt);
	}

	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
			if (argc != 3) {
				printf("kbd_m: wrong number of arguments for mouse_test_async()\n");
				return 1;
			}

			unsigned long idltm = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
			if (idltm == ULONG_MAX)
				return 1;
			printf("kbd_m::mouse_test_async(%lu)\n", idltm);
			return mouse_test_async(idltm);
	}

	else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 3) {
			printf("kbd_m: wrong number of arguments for mouse_test_gesture()\n");
			return 1;
		}

		unsigned long len = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (len == ULONG_MAX)
			return 1;
		printf("kbd_m::mouse_test_gesture(%lu)\n", len);
		return mouse_test_gesture(len);
	}

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


