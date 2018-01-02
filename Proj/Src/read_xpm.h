#ifndef __READ_XPM_H
#define __READ_XPM_H

/** @defgroup read_xpm read_xpm
 * @{
 *
 * @brief Functions for reading xpms.
 */

/**
 * @brief A structure to store xpm relevant info.
 */
typedef struct xpm{
	int width, height;
	char* pointer;  /**< @brief pointer to the memory region where the xpm is*/
	char success;  /**< @brief Whether the xpm was successfully read.*/
} xpm_t;

char* read_xpm(char *map[], int *wd, int *ht);
xpm_t read_xpm_from_file(char* filename);

/**}*/
#endif
