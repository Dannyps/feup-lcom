#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "video.h"
#include "i8042.h"

pixel_t blue_c	=	{0x00, 0x00, 0xff};
pixel_t red_c	=	{0xff, 0x00, 0x00};
pixel_t green_c =	{0xff, 0xff, 0x00};
pixel_t white_c	=	{0xff, 0xff, 0xff};
pixel_t black_c	=	{0x00, 0x00, 0x00};

pixel_t colours[] = {{0x00, 0x00, 0x00}, {0xff, 0x00, 0x00}, {0xff, 0xff, 0x00}, {0x00, 0x00, 0xff}, {0xff, 0xff, 0xff}};

int fill_screen(pixel_t color){
	printf("filling screen with color %2x%2x%2x.\n", color.r, color.g, color.b);
	static video_info_t vi;
	vi = get_vi();
	//TODO add check if video mode is on
	int i;
	for(i=0; i<vi.x*vi.y*vi.bpp/8 ; i+=3){
		vi.vm[i+0]=color.b;
		vi.vm[i+1]=color.r;
		vi.vm[i+2]=color.g;
	}
	printf("done\n");
	return 0;
}

void setP(unsigned long x, unsigned long y, pixel_t color){
	static video_info_t vi;
	vi = get_vi();
	if(vi.vm==0){
		printf("Can't write pixel if the screen is not in video mode! Exiting...\n");
		exit(-7);
	}else if(x > vi.x || y > vi.y){
		printf("setP refusing to write outside of the screen!\n");
		exit(-8);
	}
	unsigned pos=(y*vi.x+x)*vi.bpp/8;
	vi.vm[pos+0]=color.r;
	vi.vm[pos+1]=color.g;
	vi.vm[pos+2]=color.b;
	return;
}

void video_start(){
	video_m=vg_init(0x105);
}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {

        // change video mode to 0x105

        video_start();

        // print xpm here

        int width, height;

        char* pix = read_xpm(xpm, &width, &height);

        width += xi;

        height += yi;

        int initial_x = xi;

        int xpm_counter = 0;

        // printing

        while(yi < height) {

                xi = initial_x;

                while(xi < width) {
                		// colours[pix[xpm_counter]]
                        setP(xi, yi, blue_c);

                        xi++;

                        xpm_counter++;

                }

                yi++;

        }

        //video_dump_fb();

        // ESC key scan

        kbd_test_scan();

        free(pix);

        // exit video mode

        vg_exit();

        return 0;

}

char *read_xpm(char *map[], int *wd, int *ht)
{
  int width, height, colors;
  char sym[256];
  int  col;
  int i, j;
  char *pix, *pixtmp, *tmp, *line;
  char symbol;

  static video_info_t vi;
  vi = get_vi();

  /* read width, height, colors */
  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, colors);
#endif
  if (width > vi.x || height > vi.y || colors > 256) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  for (i=0; i<256; i++)
    sym[i] = 0;

  /* read symbols <-> colors */
  for (i=0; i<colors; i++) {
    if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
      return NULL;
    }
#ifdef DEBUG
    printf("%c %d\n", symbol, col);
#endif
    if (col > 256) {
      printf("read_xpm: incorrect color at line %d\n", i+1);
      return NULL;
    }
    sym[col] = symbol;
  }

  /* allocate pixmap memory */
  pix = pixtmp = malloc(width*height);

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[colors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
      tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
      if (tmp == NULL) {
    	  printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
    	  return NULL;
      }
      *pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
      printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }

  return pix;
}

