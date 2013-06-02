# $Id: Makefile,v 1.2 2005/07/25 06:51:34 magicyang Exp $
#
# Makefile for crc32 wrapper
#
# Gene Kim
# Purdue University
# October 18, 1992
#

all:	crc.o

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -rf *.o .pure

crc.o: ../../include/byteorder.h
