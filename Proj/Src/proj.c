#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/drivers.h>
#include <dirent.h>

#include "e_cal.h"

static int proc_args(int argc, char **argv);
//static unsigned long parse_ulong(char *str, int base);
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

	DIR* dir = opendir("/pr");
	if (dir)
	{
		closedir(dir);
	}
	else if (ENOENT == errno)
	{
		printf("Please run ln -s /path/to/Proj/working/copy /pr before running this app.\n");
		/* ^ in order to load xpms successfully.*/
		exit(127);
	}

	return proc_args(argc, argv);
}

// TODO: v PERSONALISE v
static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"init\"\n",
			argv[0]);
}


static int proc_args(int argc, char **argv)
{
	if(argc){
		//compiler won't complain
	}
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		printf("ECAL::init()\n");
		init();
		return 0;
	}
	else {
		printf("ECAL: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

//static unsigned long parse_ulong(char *str, int base){
//	char *endptr;
//	unsigned long val;
//
//	/* Convert string to unsigned long */
//	val = strtoul(str, &endptr, base);
//
//	/* Check for conversion errors */
//	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
//		perror("strtoul");
//		return ULONG_MAX;
//	}
//
//	if (endptr == str) {
//		printf("timer: parse_ulong: no digits were found in %s\n", str);
//		return ULONG_MAX;
//	}
//
//	/* Successful conversion */
//	return val;
//}
