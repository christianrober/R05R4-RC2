# Generated automatically from Makefile.in by configure.
# $Id: Makefile,v 1.2 2005/07/25 01:39:14 magicyang Exp $

VERSION         = 3.1.7
srcdir 		= .

install_prefix	=
prefix		= /usr
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin
sbindir		= ${exec_prefix}/sbin
mandir		= ${prefix}/man
libdir		= ${exec_prefix}/lib
man1dir		= $(mandir)/man1
man8dir		= $(mandir)/man8
docdir		= $(prefix)/doc
atdocdir	= $(docdir)/at
etcdir		= /etc

DAEMON_USERNAME	= daemon
DAEMON_GROUPNAME= daemon
LOADAVG_MX	= 0.8
ATJOB_DIR	= /var/spool/cron/atjobs
ATSPOOL_DIR	= /var/spool/cron/atspool
LN_S		= ln -s
YACC		= bison -y
LEX		= flex
LEXLIB		= -lfl

#CC 		= gcc
CFLAGS 		+= -O2
LDFLAGS		+= 
LFILE		= $(ATJOB_DIR)/.SEQ
DEFS 		= -DHAVE_CONFIG_H -DVERSION=\"$(VERSION)\" \
		-DETCDIR=\"$(etcdir)\" -DLOADAVG_MX=$(LOADAVG_MX) \
		-DDAEMON_USERNAME=\"$(DAEMON_USERNAME)\" \
		-DDAEMON_GROUPNAME=\"$(DAEMON_GROUPNAME)\" \
		-DLFILE=\"$(LFILE)\" -Wall
LIBS		= -lutil -lfl 
LIBOBJS		= 
INSTALL		= /usr/bin/install -c

CLONES		= atq atrm 
ATOBJECTS	= at.o panic.o perm.o y.tab.o lex.yy.o
RUNOBJECTS	= atd.o daemon.o $(LIBOBJS)
CSRCS   	= at.c atd.c panic.c perm.c daemon.c getloadavg.c \
			y.tab.c y.tab.h lex.yy.c
HEADERS 	= at.h panic.h parsetime.h perm.h daemon.h getloadavg.h\
			privs.h

OTHERS		= parsetime.l parsetime.y

DOCS =  Problems Copyright README ChangeLog timespec

MISC =  COPYING  Makefile.in configure acconfig.h install-sh \
	README atrun.in at.1.in atrun.8.in atd.8.in configure.in \
	config.h.in config.guess config.sub batch.in at.deny \
	$(DOCS)

DIST = $(CSRCS) $(HEADERS) $(MISC) $(OTHERS)
LIST = Filelist Filelist.asc

.PHONY: all install clean dist distclean

all: at atd atrun

at: $(ATOBJECTS)
	$(CC) $(CFLAGS) -o at $(ATOBJECTS) $(LIBS) $(LEXLIB)
	rm -f $(CLONES)
	$(LN_S) -f at atq
	$(LN_S) -f at atrm

atd: $(RUNOBJECTS)
	$(CC) $(CFLAGS) -o atd $(RUNOBJECTS) $(LIBS)

y.tab.c y.tab.h: parsetime.y
	$(YACC) -d parsetime.y

lex.yy.c: parsetime.l
	$(LEX) -i parsetime.l

atrun: atrun.in
	configure

.c.o:
	$(CC) -c $(CFLAGS) $(DEFS) $*.c

install: all
	$(INSTALL) -g root -o root -m 755 -d $(IROOT)$(etcdir)
	$(INSTALL) -g root -o root -m 755 -d $(IROOT)$(bindir)
	$(INSTALL) -g root -o root -m 755 -d $(IROOT)$(sbindir)
	$(INSTALL) -g root -o root -m 755 -d $(IROOT)$(docdir)
	$(INSTALL) -g root -o root -m 755 -d $(IROOT)$(atdocdir)
	$(INSTALL) -m 755 -d $(IROOT)$(ATJOB_DIR)
	$(INSTALL) -g $(DAEMON_GROUPNAME) -o $(DAEMON_USERNAME) -m 755 -d $(IROOT)$(ATSPOOL_DIR)
	chmod 700 $(IROOT)$(ATJOB_DIR) $(IROOT)$(ATSPOOL_DIR)
	chown $(DAEMON_USERNAME):$(DAEMON_GROUPNAME) $(IROOT)$(ATJOB_DIR) $(IROOT)$(ATSPOOL_DIR)
	touch $(IROOT)$(LFILE)
	chmod 600 $(IROOT)$(LFILE)
	chown $(DAEMON_USERNAME):$(DAEMON_GROUPNAME) $(IROOT)$(LFILE)
	test -f $(IROOT)$(etcdir)/at.allow || test -f $(IROOT)$(etcdir)/at.deny || $(INSTALL) -o root -m 600 at.deny $(IROOT)$(etcdir)/
	$(INSTALL) -g root -o root -m 4755 -s at $(IROOT)$(bindir)
	$(LN_S) -f at $(IROOT)$(bindir)/atq
	$(LN_S) -f at $(IROOT)$(bindir)/atrm
	$(INSTALL) -g root -o root -m 755 batch $(IROOT)$(bindir)
	$(INSTALL) -d -o root -g root -m 755 $(IROOT)$(man1dir)
	$(INSTALL) -d -o root -g root -m 755 $(IROOT)$(man8dir)
	$(INSTALL) -g root -o root -m 755 -s atd $(IROOT)$(sbindir)
	$(INSTALL) -g root -o root -m 755 atrun $(IROOT)$(sbindir)
	$(INSTALL) -g root -o root -m 644 at.1 $(IROOT)$(man1dir)/
	cd $(IROOT)$(man1dir) && $(LN_S) -f at.1 atq.1 && $(LN_S) -f at.1 batch.1 && $(LN_S) -f at.1 atrm.1
	$(INSTALL) -g root -o root -m 644 atd.8 $(IROOT)$(man8dir)/
	$(INSTALL) -g root -o root -m 644 atrun.8 $(IROOT)$(man8dir)/
	$(INSTALL) -g root -o root -m 644 $(DOCS) $(IROOT)$(atdocdir)
	rm -f $(IROOT)$(mandir)/cat1/at.1* $(IROOT)$(mandir)/cat1/batch.1* \
		$(IROOT)$(mandir)/cat1/atq.1*
	rm -f $(IROOT)$(mandir)/cat1/atd.8*

romfs:
	$(ROMFSINST) /bin/at
	$(ROMFSINST) /bin/atd
	$(ROMFSINST) /bin/atrun

dist: checkin $(DIST) $(LIST) Filelist.asc
	(cd ..; tar cf - `for a in $(DIST) $(LIST); do echo at-$(VERSION)/$$a; done` |\
		gzip -9 > at-$(VERSION).tar.gz)

pre: checkin $(DIST) $(LIST)
	(cd ..; tar cf - `for a in $(DIST); do echo at-$(VERSION)/$$a; done` |\
		gzip -9 > at-$(VERSION).tar.gz)
	mv ../at-$(VERSION).tar.gz ../at-$(VERSION)-`date +%Y%m%d`.tar.gz

clean:
	rm -f subs.sed *.o *.s at atd core a.out *~ $(CLONES) *.bak stamp-built

distclean: clean 
	rm -rf at.1 atd.8 atrun.8 config.cache atrun config.h \
		config.status Makefile debian/tmp config.log build

checkin: $(DIST)
	test -d RCS && for a in $(DIST); do ci -l $$a ; done

Filelist: $(DIST)
	md5sum $(DIST) > Filelist

Filelist.asc: Filelist
	pgp -sba Filelist

.depend: $(CSRCS)
	gcc $(CFLAGS) $(DEFS) -MM $(CSRCS) > .depend

at.o: at.c config.h at.h panic.h parsetime.h perm.h privs.h
atd.o: atd.c config.h privs.h daemon.h getloadavg.h
panic.o: panic.c config.h panic.h at.h
parsetime.o: parsetime.c config.h at.h panic.h
perm.o: perm.c config.h privs.h at.h
daemon.o: daemon.c config.h daemon.h privs.h
getloadavg.o: getloadavg.c config.h getloadavg.h
