# Makefile for lab5

COMPILER_TYPE= gnu

CC= gcc

PROG= lab5
SRCS= lab5.c test5.c video_test.c video_gr.c vbe.c

CFLAGS= -Wall -Wextra

DPADD+=  ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
