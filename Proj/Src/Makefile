# Makefile for lab5

COMPILER_TYPE= gnu

CC= gcc

PROG= proj
SRCS= proj.c e_cal.c video_gr.c vbe.c video.c keyboard.c timer.c read_xpm.c view.c utils.c mouse.c rtc.c

CFLAGS= -Wall -Wextra -D DOUBLEBUFF

DPADD+=  ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
