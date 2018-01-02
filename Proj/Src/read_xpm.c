#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "i8042.h"
#include "vbe.h"
#include "video_gr.h"
#include "video.h"
#include <errno.h>
#include "read_xpm.h"

char *read_xpm(char *map[], int *wd, int *ht)
{
  int width, height, ncolors;
  int i, j;
  char *pix, *pixtmp, *line;
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
		  exit(-4);
		  return NULL;
	  }
	  //printf("symbol: %d - %c\n", symbol, symbol);

	  colors[(int) symbol].r=R;
	  colors[(int) symbol].g=G;
	  colors[(int) symbol].b=B;

	  //printf("assigned to %d.\n", symbol);
#ifdef DEBUG
	  printf("-> %c %2x|%2x|%2x\n", symbol, colors[i].r, colors[i].g, colors[i].b);
#endif
  }

  for(i=0;i<256;i++){
	//printf("%d:	#%2x%2x%2x\n", i, colors[i].r, colors[i].g, colors[i].b);
  }
  /* allocate pixmap memory */
  pix = malloc(width*height*3);
  pixtmp = pix;

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[ncolors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
	  //printf("line[%d] is %c -> %2x%2x%2x\n", j, line[j], colors[(int) line[j]].r, colors[(int) line[j]].g, colors[(int) line[j]].b);
      //printf("found in pos %d\n", line[j]);
      *pixtmp = colors[(int) line[j]].r; pixtmp++;
      *pixtmp = colors[(int) line[j]].g; pixtmp++;
      *pixtmp = colors[(int) line[j]].b; pixtmp++;
#ifdef DEBUG
      //printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }
//  for(i=0;i<width*height*3;i+=3){
//	  printf("#%2x%2x%2x ", (unsigned char) pix[i], (unsigned char) pix[i+1], (unsigned char) pix[i+2]);
//  }
  free(colors);
  return pix;
}

xpm_t read_xpm_from_file(char* filename){
	int i;
	xpm_t ret;
	ret.success=0;
	if(filename==NULL){
		return ret; //could not parse *filename*.
	}

	// open file
	printf("opening %s\n", filename);
	FILE* fp = fopen(filename, "r");
	if(fp==NULL){
		printf("Error: %s\n", strerror(errno));
		exit(9);
	}

	//count lines in file
	unsigned short lines=0;
	while(1){
		char r=getc(fp);
		//printf("got %c\n", r);
		if(r==EOF)
			break;
		if(r=='\n')
			lines++;
	}
	rewind(fp);

	// create the classic array of pointers (one for each line).
	char** fa; //file_array
	fa=malloc(sizeof(char*)*lines);
	unsigned short cc=0;
	char firstline=1;

	// start reading the lines
	while(1){
		unsigned short c=0;
		char r;
		char buf[2048]; // read to this buffer first.
		memset(buf, 0, 2048);
		while(1){
			r=getc(fp);
			if(r!='\n' && r!=EOF){
				if(r=='"'){
					continue;
				}
				buf[c++]=r;
				//printf("Storing char %d-%c in pos %d.\n", r, r, c-1);
			}else{
				break;
			}
		}
		if(firstline){
			firstline=0;
			continue;
		}
		buf[strlen(buf)-1]='\0';
		//printf("line read: %s\n", buf);
		fa[cc]=malloc(sizeof(char)*(strlen(buf)+1));
		memset(fa[cc], 0, strlen(buf)+1);
		memcpy(fa[cc], buf, c);
		cc++;
		if(r==EOF)
			break;
	}
	fclose(fp);
	printf("Requesting xpm conversion... ");
	char* result=read_xpm(fa, &(ret.width), &(ret.height));
	printf("read_xpm returned 0x%x.\n", result);

	puts("done.\n");

	for(i=0;i<cc-1;i++){
		free(fa[i]);
	}

	free(fa);
	printf("mem freed\n");
	ret.pointer=result;
	ret.success=1;
	return ret;
}
