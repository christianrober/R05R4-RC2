 /*
 * File:        arch/blackfin/mach-common/lock.S
 * Based on:
 * Author:      LG Soft India
 *              COPYRIGHT 2005 Analog Devices
 * Created:     ?
 * Description: kernel locks
 *
 * Rev:          $Id: lock.S,v 1.4 2005/10/28 04:02:10 magicyang Exp $
 *
 * Modified:
 *
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

#include <linux/linkage.h>
#include <asm/cplb.h>
#include <asm/blackfin.h>

.text

#ifdef CONFIG_BLKFIN_CACHE_LOCK

.global _cache_grab_lock
.type	_cache_grab_lock, STT_FUNC;

.global _cache_lock
.type	_cache_lock, STT_FUNC;

/* When you come here, it is assumed that
 * R0 - Which way to be locked
 */

ENTRY(_cache_grab_lock)

	[--SP]=( R7:0,P5:0 );

	P1.H = (IMEM_CONTROL >> 16);
	P1.L = (IMEM_CONTROL & 0xFFFF);
	P5.H = (ICPLB_ADDR0 >> 16);
	P5.L = (ICPLB_ADDR0 & 0xFFFF);
	P4.H = (ICPLB_DATA0 >> 16);
	P4.L = (ICPLB_DATA0 & 0xFFFF);
	R7 = R0;

	/* If the code of interest already resides in the cache
	 * invalidate the entire cache itself.
  	 * invalidate_entire_icache;
	 */

	SP += -12;
	[--SP] = RETS;
	CALL _invalidate_entire_icache;
	RETS = [SP++];
	SP += 12;

	/* Disable the Interrupts*/

	CLI R3;

LOCK_WAY:

	/* Way0 - 0xFFA133E0
	 * Way1 - 0xFFA137E0
	 * Way2 - 0xFFA13BE0	Total Way Size = 4K
	 * Way3 - 0xFFA13FE0
	 */

	/* Procedure Ex. -Set the locks for other ways by setting ILOC[3:1]
	 * Only Way0 of the instruction cache can now be
	 * replaced by a new code
	 */

	R5 = R7;
	CC = BITTST(R7,0);
	IF CC JUMP CLEAR1;
	R7 = 0;
	BITSET(R7,0);
	JUMP DONE1;

CLEAR1:
	R7 = 0;
	BITCLR(R7,0);
DONE1:	R4 = R7 << 3;
	R7 = [P1];
	R7 = R7 | R4;
	SSYNC;		/* SSYNC required writing to IMEM_CONTROL. */
	.align 8;
	[P1] = R7;
	SSYNC;

	R7 = R5;
	CC = BITTST(R7,1);
	IF CC JUMP CLEAR2;
	R7 = 0;
	BITSET(R7,1);
	JUMP DONE2;

CLEAR2:
	R7 = 0;
	BITCLR(R7,1);
DONE2:	R4 = R7 << 3;
	R7 = [P1];
	R7 = R7 | R4;
	SSYNC;		/* SSYNC required writing to IMEM_CONTROL. */
	.align 8;
	[P1] = R7;
	SSYNC;

	R7 = R5;
	CC = BITTST(R7,2);
	IF CC JUMP CLEAR3;
	R7 = 0;
	BITSET(R7,2);
	JUMP DONE3;
CLEAR3:
	R7 = 0;
	BITCLR(R7,2);
DONE3:	R4 = R7 << 3;
	R7 = [P1];
	R7 = R7 | R4;
	SSYNC;		/* SSYNC required writing to IMEM_CONTROL. */
	.align 8;
	[P1] = R7;
	SSYNC;


	R7 = R5;
	CC = BITTST(R7,3);
	IF CC JUMP CLEAR4;
	R7 = 0;
	BITSET(R7,3);
	JUMP DONE4;
CLEAR4:
	R7 = 0;
	BITCLR(R7,3);
DONE4:	R4 = R7 << 3;
	R7 = [P1];
	R7 = R7 | R4;
	SSYNC;		/* SSYNC required writing to IMEM_CONTROL. */
	.align 8;
	[P1] = R7;
	SSYNC;

	STI R3;

	( R7:0,P5:0 ) = [SP++];

	RTS;

/* After the execution of critical code, the code is now locked into
 * the cache way. Now we need to set ILOC.
 *
 * R0 - Which way to be locked
 */

_cache_lock:

	[--SP]=( R7:0,P5:0 );

	P1.H = (IMEM_CONTROL >> 16);
	P1.L = (IMEM_CONTROL & 0xFFFF);

	/* Disable the Interrupts*/
	CLI R3;

	R7 = [P1];
	R2 = 0xFF87;
	R7 = R7 & R2;
	R0 = R0 << 3;
	R7 = R0 | R7;
	SSYNC;		/* SSYNC required writing to IMEM_CONTROL. */
	.align 8;
	[P1] = R7;
	SSYNC;
	/* Renable the Interrupts */
	STI R3;

	( R7:0,P5:0 ) = [SP++];
	RTS;

#endif	/*BLKFIN_CACHE_LOCK*/

/* Return the ILOC bits of IMEM_CONTROL
 */

.global _read_iloc
.type _read_iloc, STT_FUNC;

_read_iloc:

	P1.H = (IMEM_CONTROL >> 16);
	P1.L = (IMEM_CONTROL & 0xFFFF);
	R1 = 0xF;
	R0 = [P1];
	R0 = R0 >> 3;
	R0 = R0 & R1;

	RTS;
