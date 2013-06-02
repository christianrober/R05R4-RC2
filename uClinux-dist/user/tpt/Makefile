# $Header: /cvsroot/uclinux533/uClinux-dist/user/tpt/Makefile,v 1.2 2005/07/25 06:49:07 magicyang Exp $

CFLAGS+=-Wall -Wshadow

OFILES = tpt.o util.o build.o dump.o massage.o

tpt: $(OFILES)

romfs:
	$(ROMFSINST) /bin/tpt

$(OFILES): tpt.h

clean:
	rm -f $(OFILES) tpt
