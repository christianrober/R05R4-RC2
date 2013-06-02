#
#  Makefile for the Squid PAM authentication helper
#
#  $Id: Makefile.am,v 1.2 2005/07/25 06:29:21 magicyang Exp $
#
#  Uncomment and customize the following to suit your needs:
#

INCLUDES	= -I$(top_srcdir)/include

man_MANS		= pam_auth.8
EXTRA_DIST		= pam_auth.8
libexec_PROGRAMS	= pam_auth
LDADD			= -L$(top_builddir)/lib -lmiscutil -lpam $(XTRA_LIBS)
