/*
 * File:         arch/blackfin/mm/init.c
 * Based on:
 * Author:
 *
 * Created:
 * Description:
 * Rev:          $Id: init.c,v 1.13 2005/10/26 03:16:05 magicyang Exp $
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

#include <linux/swap.h>
#include <linux/bootmem.h>

#undef DEBUG

extern void free_initmem(void);
extern void l1sram_init(void);
extern void l1_data_A_sram_init(void);

/*
 * BAD_PAGE is the page that is used for page faults when linux
 * is out-of-memory. Older versions of linux just did a
 * do_exit(), but using this instead means there is less risk
 * for a process dying in kernel mode, possibly leaving a inode
 * unused etc..
 *
 * BAD_PAGETABLE is the accompanying page-table: it is initialized
 * to point to BAD_PAGE entries.
 *
 * ZERO_PAGE is a special page that is used for zero-initialized
 * data and COW.
 */
static unsigned long empty_bad_page_table;

static unsigned long empty_bad_page;

unsigned long empty_zero_page;

void show_mem(void)
{
	unsigned long i;
	int free = 0, total = 0, reserved = 0, shared = 0;

	int cached = 0;
	printk(KERN_INFO "\nMem-info:\n");
	show_free_areas();
	i = max_mapnr;
	while (i-- > 0) {
		total++;
		if (PageReserved(mem_map + i))
			reserved++;
		else if (PageSwapCache(mem_map + i))
			cached++;
		else if (!page_count(mem_map + i))
			free++;
		else
			shared += page_count(mem_map + i) - 1;
	}
	printk(KERN_INFO "%d pages of RAM\n", total);
	printk(KERN_INFO "%d free pages\n", free);
	printk(KERN_INFO "%d reserved pages\n", reserved);
	printk(KERN_INFO "%d pages shared\n", shared);
	printk(KERN_INFO "%d pages swap cached\n", cached);
}

extern unsigned long memory_start;
extern unsigned long memory_end;

/*
 * paging_init() continues the virtual memory environment setup which
 * was begun by the code in arch/head.S.
 * The parameters are pointers to where to stick the starting and ending
 * addresses  of available kernel virtual memory.
 */
void paging_init(void)
{
	/*
	 * make sure start_mem is page aligned,  otherwise bootmem and
	 * page_alloc get different views og the world
	 */
#ifdef DEBUG
	unsigned long start_mem = PAGE_ALIGN(memory_start);
#endif
	unsigned long end_mem = memory_end & PAGE_MASK;

#ifdef DEBUG
	printk("start_mem is %#lx\nvirtual_end is %#lx\n", start_mem, end_mem);
#endif

	/*
	 * initialize the bad page table and bad page to point
	 * to a couple of allocated pages
	 */
	empty_bad_page_table = (unsigned long)alloc_bootmem_pages(PAGE_SIZE);
	empty_bad_page = (unsigned long)alloc_bootmem_pages(PAGE_SIZE);
	empty_zero_page = (unsigned long)alloc_bootmem_pages(PAGE_SIZE);
	memset((void *)empty_zero_page, 0, PAGE_SIZE);

	/*
	 * Set up SFC/DFC registers (user data space)
	 */
	set_fs(USER_DS);

#ifdef DEBUG
	printk(KERN_DEBUG
	       "free_area_init -> start_mem is %#lx  virtual_end is %#lx\n",
	       start_mem, end_mem);
#endif

	{
		unsigned long zones_size[MAX_NR_ZONES] = { 0, 0, 0 };

		zones_size[ZONE_DMA] = 0;
		zones_size[ZONE_NORMAL] = (end_mem - PAGE_OFFSET) >> PAGE_SHIFT;
#ifdef CONFIG_HIGHMEM
		zones_size[ZONE_HIGHMEM] = 0;
#endif
		free_area_init(zones_size);
	}
}

void mem_init(void)
{
	unsigned int codek = 0, datak = 0, initk = 0;
	unsigned long tmp;
	extern char _etext, _stext, _sdata, _ebss, __init_begin, __init_end;
	extern unsigned int _ramend, _rambase;
	unsigned int len = _ramend - _rambase;
	unsigned long start_mem = memory_start;
	unsigned long end_mem = memory_end;

	end_mem &= PAGE_MASK;
	high_memory = (void *)end_mem;

	start_mem = PAGE_ALIGN(start_mem);
	max_mapnr = num_physpages = MAP_NR(high_memory);
	printk("Physical pages: %lx\n", num_physpages);

	/* this will put all memory onto the freelists */
	totalram_pages = free_all_bootmem();

	codek = (&_etext - &_stext) >> 10;
	datak = (&_ebss - &_sdata) >> 10;
	initk = (&__init_end - &__init_begin) >> 10;

	tmp = nr_free_pages() << PAGE_SHIFT;
	printk
	    ("Memory available: %luk/%uk RAM, (%uk init code, %uk kernel code, %uk data, %luk dma)\n",
	     tmp >> 10, len >> 10, initk, codek, datak,
	     (_ramend - memory_end) >> 10);
	/*Initialize the blackfin L1 Memory */
	l1sram_init();
	l1_data_A_sram_init();
}

#ifdef CONFIG_BLK_DEV_INITRD
void free_initrd_mem(unsigned long start, unsigned long end)
{
	int pages = 0;
	for (; start < end; start += PAGE_SIZE) {
		ClearPageReserved(virt_to_page(start));
		set_page_count(virt_to_page(start), 1);
		free_page(start);
		totalram_pages++;
		pages++;
	}
	printk(KERN_NOTICE "Freeing initrd memory: %dk freed\n", pages);
}
#endif

void free_initmem()
{
#ifdef CONFIG_RAMKERNEL
	unsigned long addr;
	extern char __init_begin, __init_end;
/*
 *	the following code should be cool even if these sections
 *	are not page aligned.
 */
	addr = PAGE_ALIGN((unsigned long)(&__init_begin));
	/* next to check that the page we free is not a partial page */
	for (; addr + PAGE_SIZE < (unsigned long)(&__init_end);
	     addr += PAGE_SIZE) {
		ClearPageReserved(virt_to_page(addr));
		set_page_count(virt_to_page(addr), 1);
		free_page(addr);
		totalram_pages++;
	}
	printk(KERN_NOTICE
	       "Freeing unused kernel memory: %ldk freed (0x%x - 0x%x)\n",
	       (addr - PAGE_ALIGN((long)&__init_begin)) >> 10,
	       (int)(PAGE_ALIGN((unsigned long)(&__init_begin))),
	       (int)(addr - PAGE_SIZE));
#endif
}
