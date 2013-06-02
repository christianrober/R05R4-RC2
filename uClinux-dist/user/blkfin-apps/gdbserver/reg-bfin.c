/* *INDENT-OFF* */ /* THIS FILE IS GENERATED */

/* A register protocol for GDB, the GNU debugger.
   Copyright 2001, 2002 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* This file was created with the aid of ``regdat.sh'' and ``./../regformats/reg-bfin.dat''.  */

#include "regdef.h"
#include "regcache.h"

struct reg regs_bfin[] = {
  { "r0", 0, 32 },
  { "r1", 32, 32 },
  { "r2", 64, 32 },
  { "r3", 96, 32 },
  { "r4", 128, 32 },
  { "r5", 160, 32 },
  { "r6", 192, 32 },
  { "r7", 224, 32 },
  { "p0", 256, 32 },
  { "p1", 288, 32 },
  { "p2", 320, 32 },
  { "p3", 352, 32 },
  { "p4", 384, 32 },
  { "p5", 416, 32 },
  { "sp", 448, 32 },
  { "fp", 480, 32 },
  { "i0", 512, 32 },
  { "i1", 544, 32 },
  { "i2", 576, 32 },
  { "i3", 608, 32 },
  { "m0", 640, 32 },
  { "m1", 672, 32 },
  { "m2", 704, 32 },
  { "m3", 736, 32 },
  { "b0", 768, 32 },
  { "b1", 800, 32 },
  { "b2", 832, 32 },
  { "b3", 864, 32 },
  { "l0", 896, 32 },
  { "l1", 928, 32 },
  { "l2", 960, 32 },
  { "l3", 992, 32 },
  { "a0x", 1024, 32 },
  { "a0w", 1056, 32 },
  { "a1x", 1088, 32 },
  { "a1w", 1120, 32 },
  { "astat", 1152, 32 },
  { "rets", 1184, 32 },
  { "lc0", 1216, 32 },
  { "lt0", 1248, 32 },
  { "lb0", 1280, 32 },
  { "lc1", 1312, 32 },
  { "lt1", 1344, 32 },
  { "lb1", 1376, 32 },
  { "cycles", 1408, 32 },
  { "cycles2", 1440, 32 },
  { "usp", 1472, 32 },
  { "seqstat", 1504, 32 },
  { "syscfg", 1536, 32 },
  { "reti", 1568, 32 },
  { "retx", 1600, 32 },
  { "retn", 1632, 32 },
  { "rete", 1664, 32 },
  { "pc", 1696, 32 },
  { "cc", 1728, 32 },
  { "extra1", 1760, 32 },
  { "extra2", 1792, 32 },
  { "extra3", 1824, 32 },
  { "ipend", 1856, 32 },
};

const char *expedite_regs_bfin[] = { "pc", "sp", "fp", 0 };

void
init_registers ()
{
    set_register_cache (regs_bfin,
			sizeof (regs_bfin) / sizeof (regs_bfin[0]));
    gdbserver_expedite_regs = expedite_regs_bfin;
}
