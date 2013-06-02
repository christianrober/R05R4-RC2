# $Id: Makefile,v 1.1 2005/07/25 05:52:02 magicyang Exp $
#
# example module makefile
#
# 
# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=jabber.so

# extra debug messages
DEFS+=-DXJ_EXTRA_DEBUG
# -DHAVE_IHTTP

# expat.h location
DEFS+=-I$(LOCALBASE)/include -I/usr/pkg/include
LIBS=-L$(LOCALBASE)/lib -L/usr/pkg/lib -lexpat

include ../../Makefile.modules
