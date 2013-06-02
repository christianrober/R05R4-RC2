#
#  Makefile for the Squid Object Cache server
#
#  $Id: Makefile.am,v 1.2 2005/07/25 06:29:22 magicyang Exp $
#
#

libexec_PROGRAMS = yp_auth
yp_auth_SOURCES	= yp_auth.c nis_support.h nis_support.c
LDADD	= -L$(top_builddir)/lib -lmiscutil $(CRYPTLIB) $(XTRA_LIBS)
INCLUDES	= -I$(top_srcdir)/include
