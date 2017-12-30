#ifndef __READ_XPM_H
#define __READ_XPM_H

typedef struct xpm{
	int width, height;
	char* pointer;
	char success;
} xpm_t;

char* read_xpm(char *map[], int *wd, int *ht);
xpm_t read_xpm_from_file(char* filename);

#endif
