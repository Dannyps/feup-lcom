# Makefile for lab5

COMPILER_TYPE= gnu

CC= gcc

PROG= project
SRCS= proj.c e_cal.c video_gr.c vbe.c video.c

CFLAGS= -Wall -Wextra

DPADD+=  ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
