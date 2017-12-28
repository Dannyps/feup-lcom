# Makefile for lab5

COMPILER_TYPE= gnu

CC= gcc

PROG= proj
SRCS= proj.c e_cal.c video_gr.c vbe.c video.c keyboard.c timer.c read_xpm.c utils.c view.c mouse.c

CFLAGS= -Wall -Wextra -D DOUBLEBUFF

DPADD+=  ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
