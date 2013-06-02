# $Id: Makefile,v 1.1 2005/07/25 05:52:00 magicyang Exp $
#
# example module makefile
#
# 
# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=cpl-c.so

DEFS +=-I/usr/include/libxml2

LIBS= -L$(LOCALBASE)/lib -L/usr/pkg/lib -lxml2

include ../../Makefile.modules
