# $Id: Makefile,v 1.2 2005/07/25 02:04:18 magicyang Exp $

CFLAGS	+=  -MD

PROG	:= frob-led
HEAD	:= keywords.h
SCRIPT	:= keywords.pl

LEDMAN	:= ${ROOTDIR}/${LINUXDIR}/include/linux/ledman.h

all: ${HEAD} ${PROG}

clean:
	rm -f ${PROG} *.o *.d ${HEAD}

${PROG}: $(patsubst %.c, %.o, $(wildcard *.c))
	${CC} ${LDFLAGS} -o $@ $^ ${LDLIBS}

-include *.d

${HEAD}: ${LEDMAN} ${SCRIPT}
	perl ${SCRIPT} <${LEDMAN} >$@

romfs:
	$(ROMFSINST) /bin/frob-led
