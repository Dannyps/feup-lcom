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
	atexit(exit_handler);
	printf("\n--- STARTED\n");

	fflush(stdout);


	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

// TODO: v PERSONALISE v
static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"cal\"\n");
}


/* GCP <= Graphics Card Program */

static int proc_args(int argc, char **argv)
{
	if (strncmp(argv[1], "cal", strlen("cal")) == 0) {
			unsigned long delay;
			unsigned mode;
			if (argc != 2+2) {
				printf("GCP: wrong no. of arguments for video_test_init()\n");
				return 1;
			}

			//e_cal function here

			//video_test_init(mode, delay);

			return 0;
		}



	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		unsigned long delay;
		unsigned mode;
		if (argc != 2+2) {
			printf("GCP: wrong no. of arguments for video_test_init()\n");
			return 1;
		}

		delay = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (delay == ULONG_MAX)
			return 1;

		int ret=sscanf(argv[2], "0x%x", &mode);
		if(ret != 1){
			printf("GCP: mode must be specified in the following format: 0x105\n");
			return 1;
		}

		printf("GCP::video_test_init(0x%x, %u)\n", mode, delay);
		video_test_init(mode, delay);
		return 0;
	}
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		unsigned long x, y, size, color;
		if (argc != 2+4) {
			printf("GCP: wrong no. of arguments for video_test_square()\n");
			return 1;
		}

		x = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (x == ULONG_MAX)
			return 1;

		y = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (y == ULONG_MAX)
			return 1;

		size = parse_ulong(argv[4], 10);					/* Parses string to unsigned long */
		if (size == ULONG_MAX)
			return 1;

		int ret=sscanf(argv[5], "%ul", &color);
		if(ret != 1){
			return 1;
		}
		if(color > 63) {
			printf("Color must be between 0 and 63\n");
			return 1;
		}


		printf("GCP::video_test_square(%lu, %lu, %lu, 0x%x)\n", x, y, size, color);
		return video_test_square(x, y, size, color);
	}
	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		unsigned long xi, yi, xf, yf, color;
		if (argc != 2+5) {
			printf("GCP: wrong no. of arguments for video_test_line()\n");
			return 1;
		}

		xi = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;

		yi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;

		xf = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;

		yf = parse_ulong(argv[5], 10);						/* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;

		color = parse_ulong(argv[6], 10);					/* Parses string to unsigned long */
		if (color == ULONG_MAX)
			return 1;

		printf("GCP::video_test_line(%lu, %lu, %lu, %lu, 0x%x)\n", xi, yi, xf, yf, color);
		return video_test_line(xi, yi, xf, yf, color);
	}
	else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		unsigned long xi, yi;
		char** xpm;
		if (argc != 2+3) {
			printf("GCP: wrong no. of arguments for test_xpm()\n");
			return 1;
		}

		if(strcmp(argv[2], "pic1") == 0){
			xpm=pic1;
		}
		else if(strcmp(argv[2], "pic2")== 0){
			xpm=pic2;
		}
		else if(strcmp(argv[2], "cross")== 0){
			xpm=cross;
		}
		else if(strcmp(argv[2], "pic3")== 0){
			xpm=pic3;
		}
		else if(strcmp(argv[2], "penguin")== 0){
			xpm=penguin;
		}
		else return 1;

		xi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;

		yi = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;

		printf("GCP::test_xpm(%s, %lu, %lu)\n", xpm, xi, yi);
		return test_xpm(xpm, xi, yi);
	}
	else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		unsigned long xi, xf, yf, fr, yi, speed;
		char** xpm;
		if (argc != 2+7) {
			printf("GCP: wrong no. of arguments for test_move()\n");
			return 1;
		}

		if(strcmp(argv[2], "pic1") == 0){
			xpm=pic1;
		}
		else if(strcmp(argv[2], "pic2")== 0){
			xpm=pic2;
		}
		else if(strcmp(argv[2], "cross")== 0){
			xpm=cross;
		}
		else if(strcmp(argv[2], "pic3")== 0){
			xpm=pic3;
		}
		else if(strcmp(argv[2], "penguin")== 0){
			xpm=penguin;
		}
		else return 1;

		xi = parse_ulong(argv[3], 10);						/* Parses string to unsigned long */
		if (xi == ULONG_MAX)
			return 1;

		yi = parse_ulong(argv[4], 10);						/* Parses string to unsigned long */
		if (yi == ULONG_MAX)
			return 1;

		xf = parse_ulong(argv[5], 10);						/* Parses string to unsigned long */
		if (xf == ULONG_MAX)
			return 1;

		yf = parse_ulong(argv[6], 10);						/* Parses string to unsigned long */
		if (yf == ULONG_MAX)
			return 1;

		speed = parse_ulong(argv[7], 10);					/* Parses string to unsigned long */
		if (speed == ULONG_MAX)
			return 1;

		fr = parse_ulong(argv[8], 10);						/* Parses string to unsigned long */
		if (fr == ULONG_MAX)
			return 1;

		if(xi<0 || yi<0 || xf<0 || yf<0) {
			printf("Can't place images off the screen.\n");
			return 1;
		}

		if (speed == 0) {
			printf("Speed cannot be 0\n");
			return 1;
		}

		if (fr <= 0) {
			printf("Framerate needs to be positive\n");
			return 1;
		}

		printf("GCP::test_move(%s, %lu, %lu, %lu, %lu, %lu, %lu)\n", xpm, xi, yi, xf, yf, speed, fr);
		return test_move(xpm, xi, yi, xf, yf, speed, fr);
	}
//	else if (strncmp(argv[1], "int", strlen("int")) == 0) {
//		if (argc != 3) {
//			printf("timer: wrong no of arguments for timer_test_int()\n");
//			return 1;
//		}
//		time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
//		if (time == ULONG_MAX)
//			return 1;
//		printf("timer::timer_test_int(%lu)\n", time);
//		return timer_test_int(time);
//	}
	else {
		printf("GCP: %s - no valid function!\n", argv[1]);
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
