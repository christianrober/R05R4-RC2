
PROG=ntpd

VPATH=	../../$(PROG)

# config.h
CFLAGS += -DHAVE_CONFIG_H -I../include

# ntp headers
CFLAGS += -I../../include

# ntp_resolver.c is presently unused...
SRCS=	cmd_args.c map_vme.c ntp_config.c ntp_control.c \
	ntp_crypto.c ntp_filegen.c \
	ntp_intres.c ntp_io.c ntp_loopfilter.c ntp_monitor.c ntp_peer.c \
	ntp_proto.c ntp_refclock.c ntp_request.c \
	ntp_restrict.c ntp_timer.c \
	ntp_util.c ntpd.c refclock_acts.c refclock_arbiter.c refclock_arc.c \
	refclock_as2201.c refclock_atom.c refclock_bancomm.c \
	refclock_chronolog.c refclock_chu.c refclock_conf.c refclock_datum.c \
	refclock_dumbclock.c refclock_fg.c refclock_gpsvme.c refclock_heath.c \
	refclock_hopfser.c refclock_hopfpci.c \
	refclock_hpgps.c refclock_irig.c refclock_jjy.c refclock_jupiter.c refclock_leitch.c \
	refclock_local.c refclock_msfees.c refclock_mx4200.c refclock_nmea.c \
	refclock_oncore.c refclock_palisade.c \
	refclock_parse.c refclock_pcf.c refclock_pst.c refclock_ptbacts.c \
	refclock_shm.c refclock_tpro.c refclock_trak.c refclock_true.c \
	refclock_ulink.c refclock_usno.c refclock_wwv.c refclock_wwvb.c

OBJS=	version.o $(SRCS:.c=.o)
LIBS=	../libntp/libntp.a ../libparse/libparse.a

version.c: $(SRCS)
	CSET=`cat ../../version`		;\
	DATE=`LC_TIME=C date`			;\
	echo '#include <config.h>' > version.c	;\
	echo "const char * Version = \"$(PROG) \" VERSION \"@$$CSET $$DATE (uCbuild)\";" >> version.c

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) -lm

all: $(PROG)

romfs:
	$(ROMFSINST) /bin/$(PROG)

clean:
	rm -f $(PROG) $(PROG).gdb version.c $(LIB) $(OBJS) *.gdb .version
