
SUBDIRS=C fr it ja

LINPHONE_SOUNDS=ringback.wav hello8000.wav hello16000.wav
LINPHONE_RINGS=rings/orig.wav \
				rings/oldphone.wav \
				rings/rock.wav \
				rings/bigben.wav \
				rings/toy.wav \
				rings/sweet.wav \
				rings/synth.wav \
				rings/tapping.wav

sounddir=$(datadir)/sounds/linphone

sound_DATA=$(LINPHONE_SOUNDS)

ringdir=$(datadir)/sounds/linphone/rings

ring_DATA=$(LINPHONE_RINGS)

linphone_applidir= $(prefix)/share/gnome/apps/Internet
linphone_appli_DATA= linphone.desktop

#to be compliant with freedesktop.org:
linphone_fddir= $(prefix)/share/applications/
linphone_fd_DATA= linphone.desktop


pkgconfigdir=$(prefix)/lib/pkgconfig
pkgconfig_DATA=linphone.pc

EXTRA_DIST = 					$(LINPHONE_SOUNDS) \
								$(LINPHONE_RINGS) \
						        linphone.gnorba     \
						        linphone.desktop   \
						        linphone_applet.desktop \
								linphone.pc.in \
								Makefile.inc
