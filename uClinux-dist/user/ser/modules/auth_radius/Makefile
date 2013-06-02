# $Id: Makefile,v 1.1 2005/07/25 05:52:00 magicyang Exp $
#
# Digest Authentication - Radius support
#
# 
# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=auth_radius.so
DEFS+=-I$(LOCALBASE)/include
LIBS=-L$(LOCALBASE)/lib -L/usr/pkg/lib -lradiusclient

include ../../Makefile.modules
