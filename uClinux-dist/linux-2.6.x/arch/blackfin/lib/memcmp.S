/*
 * File:         arch/blackfin/lib/memcmp.S
 * Based on:
 * Author:
 *
 * Created:
 * Description:
 *
 * Rev:          $Id: memcmp.S,v 1.7 2005/10/28 04:02:10 magicyang Exp $
 *
 * Modified:
 *               Copyright 2004-2005 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.
 * If not, write to the Free Software Foundation,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

.text
.global _memcmp;
	.align 2;
/*
 * C Library function MEMCMP
 * R0 = First Address
 * R1 = Second Address
 * R2 = count
 * Favours word aligned data.
 */
_memcmp:
	I1 = P3;
	P0 = R0;			/* P0 = s1 address */
	P3 = R1;			/* P3 = s2 Address  */
	P2 = R2 ;			/* P2 = count */
	CC = R2 <= 7(IU);
	IF CC JUMP  too_small;
	I0 = R1;			/* s2 */
	R1 = R1 | R0;		/* OR addresses together */
	R1 <<= 30;		/* check bottom two bits */
	CC =  AZ;			/* AZ set if zero. */
	IF !CC JUMP  bytes ;	/* Jump if addrs not aligned. */

	P1 = P2 >> 2;		/* count = n/4 */
	R3 =  3;
	R2 = R2 & R3;		/* remainder */
	P2 = R2;			/* set remainder */

	LSETUP (quad_loop_s , quad_loop_e) LC0=P1;
quad_loop_s:
	NOP;
	R0 = [P0++];
	R1 = [I0++];
	CC = R0 == R1;
	IF !CC JUMP quad_different;
quad_loop_e:
	NOP;

	P3 = I0;			/* s2 */
too_small:
	CC = P2 == 0;		/* Check zero count*/
	IF CC JUMP finished;	/* very unlikely*/

bytes:
	LSETUP (byte_loop_s , byte_loop_e) LC0=P2;
byte_loop_s:
	R1 = B[P3++](Z);	/* *s2 */
	R0 = B[P0++](Z);	/* *s1 */
	CC = R0 == R1;
	IF !CC JUMP different;
byte_loop_e:
	NOP;

different:
	R0 = R0 - R1;
	P3 = I1;
	RTS;

quad_different:
/* We've read two quads which don't match.
 * Can't just compare them, because we're
 * a little-endian machine, so the MSBs of
 * the regs occur at later addresses in the
 * string.
 * Arrange to re-read those two quads again,
 * byte-by-byte.
 */
	P0 += -4;		/* back up to the start of the */
	P3 = I0;		/* quads, and increase the*/
	P2 += 4;		/* remainder count*/
	P3 += -4;
	JUMP bytes;

finished:
	R0 = 0;
	P3 = I1;
	RTS;

._memcmp.end:
