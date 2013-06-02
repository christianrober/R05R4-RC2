#
#  Makefile for the Squid LDAP authentication helper
#
#  $Id: Makefile.am,v 1.2 2005/07/25 06:29:21 magicyang Exp $
#
#  Uncomment and customize the following to suit your needs:
#

libexec_PROGRAMS 	= squid_ldap_auth
man_MANS 		= squid_ldap_auth.8
EXTRA_DIST		= squid_ldap_auth.8
squid_ldap_auth_SOURCES	= squid_ldap_auth.c

LDADD = -L$(top_builddir)/lib -lmiscutil -lldap -llber $(XTRA_LIBS)
INCLUDES      = -I$(top_srcdir)/include
