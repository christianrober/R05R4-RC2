#
#  Makefile for the Squid Object Cache server
#
#  $Id: Makefile.am,v 1.2 2005/07/25 06:29:22 magicyang Exp $
#

libexec_PROGRAMS = wb_auth
wb_auth_SOURCES = wb_basic_auth.c wb_common.c wbntlm.h
INCLUDES = -I$(top_srcdir)/include -I@SAMBASOURCES@
LDADD = -L$(top_builddir)/lib -lmiscutil -lntlmauth $(XTRA_LIBS)
