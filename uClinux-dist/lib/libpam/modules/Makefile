# $Id: Makefile,v 1.3 2005/08/16 03:41:37 magicyang Exp $
#
# Makefile
#
# This makefile controls the build process of shared and static PAM modules.
#
#

include ../Make.Rules

MODDIRS=$(shell /bin/ls -d pam_*)

all:	
	@echo modules sources available are:
	@ls -d $(MODDIRS) 2>/dev/null ; echo :--------
	@echo
ifdef STATIC
	rm -f ./_static_module_*
endif
	@for i in $(MODDIRS) ; do \
		if [ -d $$i ]; then { \
		   $(MAKE) -C $$i all ; \
		   if [ $$? -ne 0 ]; then exit 1 ; fi ; \
		} elif [ -f ./.$$i ]; then { \
		   cat ./.$$i ; \
		} fi ; \
	done

download:
	@./download-all

install:
	for i in $(MODDIRS) ; do \
		if [ -d $$i ]; then { \
		$(MAKE) -C $$i install ; \
		if [ $$? -ne 0 ]; then exit 1 ; fi ; \
		} fi ; \
	done

remove:
	for i in $(MODDIRS) ; do \
		if [ -d $$i ]; then { \
			$(MAKE) -C $$i remove ; \
		} fi ; \
	done

lclean:
	rm -f _static_module_*

clean: lclean
	for i in $(MODDIRS) ; do \
		if [ -d $$i ]; then { \
			$(MAKE) -C $$i clean ; \
		} fi ; \
	done 

