# $Id: Makefile,v 1.2 2005/07/25 06:51:34 magicyang Exp $
#
# Makefile for SHA wrapper
#
# Gene Kim
# Purdue University
# August 10, 1993
#

all:	sha.o shawrapper.o

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -rf *.o .pure

sha.o: ../../include/byteorder.h
shawrapper.o: ../../include/byteorder.h
