#
#  Makefile for the Squid SASL authentication helper
#
#  $Id: Makefile.am,v 1.2 2005/07/25 06:29:21 magicyang Exp $
#
#  Uncomment and customize the following to suit your needs:
#

INCLUDES	= -I$(top_srcdir)/include

libexec_PROGRAMS	= sasl_auth
LDADD			= -L$(top_builddir)/lib -lmiscutil $(LIBSASL) $(XTRA_LIBS)
EXTRA_DIST		= squid_sasl_auth squid_sasl_auth.conf
