## $Id: Makefile.am,v 1.2 2005/07/25 03:57:11 magicyang Exp $

## Process this file with automake to produce Makefile.in

## this is so that Automake includes the C compiling definitions, and
## includes the source files in the distribution.
EXTRA_PROGRAMS = automake_dummy
automake_dummy_SOURCES = lirc_parallel.c lirc_parallel.h

## there is no *just* object file support in automake.  This is close enough
module_DATA = lirc_parallel.o

# where the kernel sources are located
KERNEL_LOCATION=@kerneldir@

# some magic for using linux kernel settings
# when compiling module(s)
CC           = @kernelcc@
MI_OBJS      = $(module_DATA)
EXTRA_CFLAGS = $(DEFS)

$(module_DATA): $(automake_dummy_SOURCES) $(top_srcdir)/config.h ../lirc.h
  ifndef MAKING_MODULES
	DIR=`pwd`; (cd $(KERNEL_LOCATION); make SUBDIRS=$$DIR obj-m=$(module_DATA) modules)
  endif
# the spaces above are necessary because automake otherwise will complain
# and even worse, it will delete the line

install-exec-local: mkdev
uninstall-local: rmdev

mkdev:
	test -c @devdir@/lirc || @mknod@ @devdir@/lirc c @lirc_major@ 0

rmdev:
	test -c @devdir@/lirc && $(RM) @devdir@/lirc

install-data-local:
	-@depmod@ -a

CLEANFILES = $(module_DATA) .$(module_DATA).flags *~

include $(KERNEL_LOCATION)/Rules.make
