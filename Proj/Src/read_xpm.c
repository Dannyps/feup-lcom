#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i8042.h"
#include "vbe.h"
#include "video_gr.h"
#include "video.h"


char *read_xpm(char *map[], int *wd, int *ht)
{
  int width, height, ncolors;
  char sym[256];
  int  col;
  int i, j;
  char *pix, *pixtmp, *tmp, *line;
  char symbol=6;

  static video_info_t vi;
  vi = get_vi();

  /* read width, height, colors */
  if (sscanf(map[0],"%d %d %d", &width, &height, &ncolors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, ncolors);
#endif
  if (width > vi.x || height > vi.y || ncolors > 256) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  pixel_t* colors = malloc(256*sizeof(pixel_t));
  memset(colors, 0, sizeof(pixel_t)*256);

  unsigned int R,G,B;

  /* read symbols <-> colors */
  for (i=0; i<ncolors; i++) {
	  if (sscanf(map[i+1], "%c %2x %2x %2x", &symbol, &R, &G, &B) != 4) {
		  printf("read_xpm: incorrect symbol, color at line %d: %s\n", i+1, map[i+1]);
		  return NULL;
	  }
	  printf("symbol: %d - %c\n", symbol, symbol);

	  colors[(int) symbol].r=R;
	  colors[(int) symbol].g=G;
	  colors[(int) symbol].b=B;

	  printf("assigned to %d.\n", symbol);
#ifdef DEBUG
	  printf("-> %c %2x|%2x|%2x\n", symbol, colors[i].r, colors[i].g, colors[i].b);
#endif
  }

  for(i=0;i<256;i++){
	//printf("%d:	#%2x%2x%2x\n", i, colors[i].r, colors[i].g, colors[i].b);
  }
  /* allocate pixmap memory */
  pix = pixtmp = malloc(width*height*3);

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[ncolors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
    	printf("line[%d] is %c -> %2x%2x%2x\n", j, line[j], colors[(int) line[j]].r, colors[(int) line[j]].g, colors[(int) line[j]].b);
      printf("found in pos %d\n", line[j]);
      *pixtmp = colors[(int) line[j]].r; pixtmp++;
      *pixtmp = colors[(int) line[j]].g; pixtmp++;
      *pixtmp = colors[(int) line[j]].b; pixtmp++;
#ifdef DEBUG
      //printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }
  for(i=0;i<width*height*3;i+=3){
	  printf("#%2x%2x%2x ", (unsigned char) pix[i], (unsigned char) pix[i+1], (unsigned char) pix[i+2]);
  }
  free(colors);
  return pix;
}
