#
#  Makefile for the Squid Object Cache server
#
#  $Id: Makefile.am,v 1.2 2005/07/25 06:29:22 magicyang Exp $
#
#  Uncomment and customize the following to suit your needs:
#


libexec_PROGRAMS = getpwname_auth

getpwname_auth_SOURCES = getpwnam_auth.c 

INCLUDES      = -I$(top_srcdir)/include
LDADD = -L$(top_builddir)/lib -lmiscutil $(CRYPTLIB) $(XTRA_LIBS)
