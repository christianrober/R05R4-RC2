#
#  Makefile for the Squid Object Cache server
#
#  $Id: Makefile.am,v 1.2 2005/07/25 06:29:23 magicyang Exp $
#
#  Uncomment and customize the following to suit your needs:
#

libexec_PROGRAMS = fakeauth_auth
fakeauth_auth_SOURCES	= fakeauth_auth.c ntlm.h
INCLUDES      = -I. -I$(top_builddir)/include -I$(top_srcdir)/include \
		-I$(top_srcdir)/src/
LDADD	= -L$(top_builddir)/lib -lmiscutil $(CRYPTLIB) $(XTRA_LIBS)
