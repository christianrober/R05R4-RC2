## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign 1.6 dist-zip

SUBDIRS = src include doc debian

m4datadir = $(datadir)/aclocal
m4data_DATA = ao.m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ao.pc

mandir = @MANDIR@
man_MANS = libao.conf.5

EXTRA_DIST = README AUTHORS CHANGES COPYING libao.spec ao.m4 acinclude.m4 ao.pc.in $(man_MANS)

debug:
	$(MAKE) all CFLAGS="@DEBUG@"

profile:
	$(MAKE) all CFLAGS="@PROFILE@"
