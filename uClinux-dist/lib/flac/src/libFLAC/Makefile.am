#  libFLAC - Free Lossless Audio Codec library
#  Copyright (C) 2001,2002,2003  Josh Coalson
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Library General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Library General Public License for more details.
#
#  You should have received a copy of the GNU Library General Public
#  License along with this library; if not, write to the
#  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
#  Boston, MA  02111-1307, USA.

lib_LTLIBRARIES = libFLAC.la
if DEBUG
DEBUGCFLAGS = -DFLAC__OVERFLOW_DETECT
endif
CFLAGS = @CFLAGS@ $(DEBUGCFLAGS)

if FLaC__NO_ASM
else
if FLaC__CPU_IA32
if FLaC__HAS_NASM
ARCH_SUBDIRS = ia32
libFLAC_la_LIBADD = ia32/libFLAC-asm.la
endif
endif
endif

SUBDIRS = $(ARCH_SUBDIRS) include .

m4datadir = $(datadir)/aclocal
m4data_DATA = libFLAC.m4

EXTRA_DIST = \
	Makefile.lite \
	libFLAC_dynamic.dsp \
	libFLAC_static.dsp \
	libFLAC.m4

# see 'http://www.gnu.org/software/libtool/manual.html#Versioning' for numbering convention
libFLAC_la_LDFLAGS = -version-info 5:2:1

libFLAC_la_SOURCES = \
	bitbuffer.c \
	bitmath.c \
	cpu.c \
	crc.c \
	file_decoder.c \
	file_encoder.c \
	fixed.c \
	format.c \
	lpc.c \
	md5.c \
	memory.c \
	metadata_iterators.c \
	metadata_object.c \
	seekable_stream_decoder.c \
	seekable_stream_encoder.c \
	stream_decoder.c \
	stream_encoder.c \
	stream_encoder_framing.c
