#
# $Id: Makefile,v 1.3 2005/08/16 03:41:36 magicyang Exp $
#
#

dummy:
	@echo "*** This is not a top level Makefile!"

##########################################################

all:
	$(MAKE) -C pam_conv1 all

install: $(FAKEROOT)$(CONFIGED)/pam.conf
	$(MAKE) -C pam_conv1 install

$(FAKEROOT)$(CONFIGED)/pam.conf: ./pam.conf
	bash -f ./install_conf

remove:
	rm -f $(FAKEROOT)$(CONFIGED)/pam.conf
	$(MAKE) -C pam_conv1 remove

check:
	bash -f ./md5itall

lclean:
	rm -f core *~ .ignore_age

clean: lclean
	$(MAKE) -C pam_conv1 clean

extraclean: lclean
	$(MAKE) -C pam_conv1 extraclean
