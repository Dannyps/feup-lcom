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
	char* pointer;  /**< @brief Pointer to the memory region where the xpm is*/
	char success;  /**< @brief Whether the xpm was successfully read.*/
} xpm_t;

/**
 * @brief Read xpm from char**
 * @warning Returned memory should be freed asap!
 * @warning This function requires map to be defined in the object file, thus making the linked file too big to run. Please use read_xpm_from_file(char* filename)!
 * @param map pointer to array
 * @param wd xpm width returned here
 * @param ht xpm height returned here
 * @return pointer to mem position that holds the video data.
 */
char* read_xpm(char *map[], int *wd, int *ht);

/**
 * @brief Read xpm from file filename
 * @warning Returned memory should be freed asap! And then again, probably shouldn't because you'll need this until the end.
 * @param filename absolute path to file
 * @return xpm_t with a pointer to the video data.
 */
xpm_t read_xpm_from_file(char* filename);

/**}*/
#endif
