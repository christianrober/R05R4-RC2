# Makefile for uClibc's sh64 optimized string routines
#
# Copyright (C) 2003 Paul Mundt <lethal@linux-sh.org>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Library General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Library General Public License for more
# details.
#
# You should have received a copy of the GNU Library General Public License
# along with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

TOPDIR=../../../
include $(TOPDIR)Rules.mak

SSRC= memcpy.S memset.S strcpy.S strlen.S
SOBJS=$(patsubst %.S,%.o, $(SSRC))

all: $(SOBJS) $(LIBC)

$(LIBC): ar-target

ar-target: $(SOBJS)
	$(AR) $(ARFLAGS) $(LIBC) $(SOBJS)

$(SOBJS): %.o : %.S
	$(CC) $(ASFLAGS) -c $< -o $@
	$(STRIPTOOL) -x -R .note -R .comment $*.o

clean:
	$(RM) *.[oa] *~ core

