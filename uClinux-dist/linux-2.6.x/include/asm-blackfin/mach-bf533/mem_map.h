
/*
 * File:         include/asm-blackfin/mach-bf533/mem_map.h
 * Based on:
 * Author:
 *
 * Created:
 * Description:
 *
 * Rev:
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

#ifndef _MEM_MAP_533_H_
#define _MEM_MAP_533_H_

#define COREMMR_BASE           0xFFE00000	// Core MMRs
#define SYSMMR_BASE            0xFFC00000	// System MMRs

/* Level 3 SDRAM Memory */
#define RAM_START		0x1000
#define RAM_LENGTH		(CONFIG_MEM_SIZE * 1024 * 1024)
#define RAM_END 		(CONFIG_MEM_SIZE * 1024 * 1024)

/* Async Memory Banks */
#define ASYNC_BANK3_BASE	0x20300000	// Async Bank 3
#define ASYNC_BANK3_SIZE	0x00100000	/* 1M */
#define ASYNC_BANK2_BASE	0x20200000	// Async Bank 2
#define ASYNC_BANK2_SIZE	0x00100000	/* 1M */
#define ASYNC_BANK1_BASE	0x20100000	// Async Bank 1
#define ASYNC_BANK1_SIZE	0x00100000	/* 1M */
#define ASYNC_BANK0_BASE	0x20000000	// Async Bank 0
#define ASYNC_BANK0_SIZE	0x00100000	/* 1M */

/* Level 1 Memory */

/* Memory Map for ADSP-BF533 processors */

#ifdef CONFIG_BF533
#define L1_CODE_START       0xFFA00000
#define L1_DATA_A_START     0xFF800000
#define L1_DATA_B_START     0xFF900000

#ifdef CONFIG_BLKFIN_CACHE
#define L1_CODE_LENGTH      (0x14000 - 0x4000)
#else
#define L1_CODE_LENGTH      0x14000
#endif

#ifdef CONFIG_BLKFIN_DCACHE
#define DMEM_CNTR (ACACHE_BCACHE | ENDCPLB | PORT_PREF0)
#define L1_DATA_A_LENGTH      (0x8000 - 0x4000)
#else
#define DMEM_CNTR (ASRAM_BSRAM | ENDCPLB | PORT_PREF0)
#define L1_DATA_A_LENGTH      0x8000
#endif

#ifdef CONFIG_BLKFIN_DCACHE
#define L1_DATA_B_LENGTH      (0x8000 - 0x4000)
#else
#define L1_DATA_B_LENGTH      0x8000
#endif
#endif

/* Memory Map for ADSP-BF532 processors */

#ifdef CONFIG_BF532
#define L1_CODE_START       0xFFA08000
#define L1_DATA_A_START     0xFF804000
#define L1_DATA_B_START     0xFF904000

#ifdef CONFIG_BLKFIN_CACHE
#define L1_CODE_LENGTH      (0xC000 - 0x4000)
#else
#define L1_CODE_LENGTH      0xC000
#endif

#ifdef CONFIG_BLKFIN_DCACHE
#define DMEM_CNTR (ACACHE_BCACHE | ENDCPLB | PORT_PREF0)
#define L1_DATA_A_LENGTH      (0x4000 - 0x4000)
#else
#define DMEM_CNTR (ASRAM_BSRAM | ENDCPLB | PORT_PREF0)
#define L1_DATA_A_LENGTH      0x4000
#endif

#ifdef CONFIG_BLKFIN_DCACHE
#define L1_DATA_B_LENGTH      (0x4000 - 0x4000)
#else
#define L1_DATA_B_LENGTH      0x4000
#endif
#endif

/* Memory Map for ADSP-BF531 processors */

#ifdef CONFIG_BF531
#define L1_CODE_START       0xFFA08000
#define L1_DATA_A_START     0xFF804000
#define L1_DATA_B_START     0xFF904000
#define L1_CODE_LENGTH      0x4000
#define L1_DATA_B_LENGTH      0x0000
#ifdef CONFIG_BLKFIN_DCACHE
#define DMEM_CNTR (ACACHE_BSRAM | ENDCPLB)
#define L1_DATA_A_LENGTH      (0x4000 - 0x4000)
#else
#define DMEM_CNTR (ASRAM_BSRAM | ENDCPLB)
#define L1_DATA_A_LENGTH      0x4000
#endif
#endif

/* Scratch Pad Memory */

#if defined(CONFIG_BF533) || defined(CONFIG_BF532) || defined(CONFIG_BF531)
#define L1_SCRATCH_START	0xFFB00000
#define L1_SCRATCH_LENGTH	0x1000
#endif

#endif				/* _MEM_MAP_533_H_ */
