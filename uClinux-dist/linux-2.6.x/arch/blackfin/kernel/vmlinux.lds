/*
 * File:	 arch/blackfin/kernel/vmlinux.lds.S
 * Based on:	 none - original work
 * Author:
 *
 * Created:	 Tue Sep 21 2004
 * Description:	 Master linker script for blackfin architecture
 *
 * Rev:	         $Id: vmlinux.lds.S,v 1.20 2005/11/02 18:10:03 jiez Exp $
 *
 * Modified:
 *               Copyright 2004-2005 Analog Devices Inc.
 *
 * Bugs:	 Enter bugs at http:	//blackfin.uclinux.org/
 *
 * This program is free software ;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation	;  either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY ;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program ;  see the file COPYING.
 * If not, write to the Free Software Foundation,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.12.1-BFIN-2005R4
 * Mon Apr 10 15:33:45 2006
 */
/*
 * Code maturity level options
 */





/*
 * General setup
 */
/*
 * Loadable module support
 */
/*
 * Blackfin Processor Options
 */

/*
 * Processor and Board Settings
 */
/*
 * BF533/2/1 Specific Configuration
 */

/*
 * Interrupt Priority Assignment
 */

/*
 * Priority
 */
/*
 * Board specific issues
 */

/*
 * Board Setup
 */





/*
 * LED Status Indicators
 */







/*
 * Console UART Setup
 */
/*
 * DMA Support
 */



/*
 * Cache Support
 */







/*
 * Clock Settings
 */







/*
 * Asynchonous Memory Configuration
 */

/*
 * EBIU_AMBCTL Global Control
 */
/*
 * EBIU_AMBCTL Control
 */






/*
 * Bus options (PCI, PCMCIA, EISA, MCA, ISA)
 */


/*
 * PCCARD (PCMCIA/CardBus) support
 */


/*
 * PCI Hotplug Support
 */

/*
 * Executable File Formats
 */





/*
 * Power management options
 */


/*
 * CPU Frequency scaling
 */


/*
 * Generic Driver Options
 */




/*
 * Memory Technology Devices (MTD)
 */
/*
 * User Modules And Translation Layers
 */






/*
 * RAM/ROM/Flash chip drivers
 */
/*
 * Mapping drivers for chip access
 */






/*
 * FLASH_EBIU_AMBCTL Control
 */






/*
 * Self-contained MTD device drivers
 */






/*
 * Disk-On-Chip Device Drivers
 */




/*
 * NAND Flash Device Drivers
 */


/*
 * Parallel port support
 */


/*
 * Plug and Play support
 */

/*
 * Block devices
 */
/*
 * IO Schedulers
 */






/*
 * ATA/ATAPI/MFM/RLL support
 */


/*
 * IDE Extra configuration
 */

/*
 * SCSI device support
 */


/*
 * Multi-device support (RAID and LVM)
 */


/*
 * Fusion MPT device support
 */

/*
 * IEEE 1394 (FireWire) support
 */

/*
 * I2O device support
 */

/*
 * Networking support
 */


/*
 * Networking options
 */
/*
 * SCTP Configuration (EXPERIMENTAL)
 */
/*
 * QoS and/or fair queueing
 */



/*
 * Network testing
 */
/*
 * Ethernet (10 or 100Mbit)
 */




/*
 * Ethernet (1000 Mbit)
 */

/*
 * Ethernet (10000 Mbit)
 */

/*
 * Token Ring devices
 */

/*
 * Wireless LAN (non-hamradio)
 */


/*
 * Wan interfaces
 */






/*
 * ISDN subsystem
 */


/*
 * Telephony Support
 */


/*
 * Input device support
 */


/*
 * Hardware I/O ports
 */



/*
 * I2C support
 */


/*
 * Character devices
 */
/*
 * Serial drivers
 */


/*
 * Non-8250 serial port support
 */







/*
 * IPMI
 */


/*
 * Watchdog Cards
 */
/*
 * Ftape, the floppy tape device driver
 */



/*
 * TPM devices
 */

/*
 * Multimedia devices
 */


/*
 * Digital Video Broadcasting Devices
 */


/*
 * File systems
 */
/*
 * XFS support
 */
/*
 * CD-ROM/DVD Filesystems
 */



/*
 * DOS/FAT/NT Filesystems
 */




/*
 * Pseudo filesystems
 */
/*
 * Miscellaneous filesystems
 */
/*
 * Network File Systems
 */
/*
 * Partition Types
 */



/*
 * Native Language Support
 */
/*
 * Graphics support
 */


/*
 * Sound
 */


/*
 * USB support
 */



/*
 * USB Gadget Support
 */


/*
 * Profiling support
 */


/*
 * Kernel hacking
 */
/*
 * Security options
 */







/*
 * Cryptographic options
 */


/*
 * Hardware crypto devices
 */

/*
 * Library routines
 */
/*
 * mem_map.h
 * Common header file for blackfin family of processors.
 *
 */







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







/* Level 3 SDRAM Memory */




/* Async Memory Banks */
/* Level 1 Memory */

/* Memory Map for ADSP-BF533 processors */
/* Memory Map for ADSP-BF532 processors */
/* Memory Map for ADSP-BF531 processors */
/* Scratch Pad Memory */
OUTPUT_FORMAT("elf32-bfin")
ENTRY(__start)
_jiffies = _jiffies_64;
MEMORY
 {
 ram : ORIGIN = 0x1000, LENGTH = (128 * 1024 * 1024)
 l1_data_a : ORIGIN = 0xFF800000, LENGTH = (0x8000 - 0x4000)
 l1_data_b : ORIGIN = 0xFF900000, LENGTH = (0x8000 - 0x4000)
 l1_code : ORIGIN = 0xFFA00000, LENGTH = (0x14000 - 0x4000)
 l1_scratch : ORIGIN = 0xFFB00000,LENGTH = 0x1000
 }

SECTIONS
{
 . = 0x1000 ;

 .text : {
   __stext = .;
  *(.text)
  ___sched_text_start = .; *(.sched.text) ___sched_text_end = .;
  *(.text.lock)
  . = ALIGN(16);
    ___start___ex_table = .;
  *(__ex_table)
   ___stop___ex_table = .;

    *($code)
           *(.rodata)
           *(.rodata.*)
    *(__vermagic) /* Kernel version magic */
           *(.rodata1)
           *(.fixup)

  /* Kernel symbol table: Normal symbols */
  . = ALIGN(4);
  ___start___ksymtab = .;
  *(__ksymtab)
  ___stop___ksymtab = .;

  /* Kernel symbol table: GPL-only symbols */
  ___start___ksymtab_gpl = .;
  *(__ksymtab_gpl)
  ___stop___ksymtab_gpl = .;

  /* Kernel symbol table: Normal symbols */
  ___start___kcrctab = .;
  *(__kcrctab)
  ___stop___kcrctab = .;

  /* Kernel symbol table: GPL-only symbols */
  ___start___kcrctab_gpl = .;
  *(__kcrctab_gpl)
  ___stop___kcrctab_gpl = .;

  /* Kernel symbol table: strings */
  *(__ksymtab_strings)

   . = ALIGN(4) ;
    __etext = . ;
 } > ram

 .init :
 {
  . = ALIGN(4096);
  ___init_begin = .;
  __sinittext = .;
  *(.init.text)
  __einittext = .;
  *(.init.data)
  . = ALIGN(16);
  ___setup_start = .;
  *(.init.setup)
  ___setup_end = .;
  ___start___param = .;
  *(__param)
  ___stop___param = .;
  ___initcall_start = .;
  *(.initcall1.init)
  *(.initcall2.init)
  *(.initcall3.init)
  *(.initcall4.init)
  *(.initcall5.init)
  *(.initcall6.init)
  *(.initcall7.init)
  ___initcall_end = .;
  ___con_initcall_start = .;
  *(.con_initcall.init)
  ___con_initcall_end = .;
  ___security_initcall_start = .;
  *(.security_initcall.init)
  ___security_initcall_end = .;
  . = ALIGN(4);
  ___initramfs_start = .;
  *(.init.ramfs)
  ___initramfs_end = .;
  . = ALIGN(4);
  ___init_end = .;
 } > ram

  __l1_lma_start = .;
 .text_l1 0xFFA00000 :
  AT ( __l1_lma_start )
 {
  . = ALIGN(4) ;
   __stext_l1 = . ;
  *(.text.l1)

  . = ALIGN(4) ;
   __etext_l1 = . ;
 }

 .data_l1 0xFF800000 :
  AT ( __l1_lma_start + SIZEOF(.text_l1) )
 {
  . = ALIGN(4) ;
   __sdata_l1 = . ;
  *(.data.l1)
   __edata_l1 = . ;

  . = ALIGN(4) ;
   __sbss_l1 = . ;
  *(.bss.l1)

  . = ALIGN(4) ;
   __ebss_l1 = . ;
 }

 . = __l1_lma_start + SIZEOF(.text_l1) + SIZEOF(.data_l1) ;
 .data (__l1_lma_start + SIZEOF(.text_l1) + SIZEOF(.data_l1)) : AT ( __l1_lma_start + SIZEOF(.text_l1) + SIZEOF(.data_l1) )
 {
   __sdata = . ;
  . = ALIGN(0x2000) ;
  *(.data.init_task)
  *(.data)
  __edata = . ;
 } > ram

 /DISCARD/ : { /* Exit code and data*/
  *(.exit.text)
  *(.exit.data)
  *(.exitcall.exit)
 } > ram


 .bss : AT ( __l1_lma_start + SIZEOF(.text_l1) + SIZEOF(.data_l1) + SIZEOF(.data) )
 {
  . = ALIGN(4) ;
   __sbss = . ;
  *(.bss)
  *(COMMON)
  . = ALIGN(4) ;
   __ebss = . ;
   __end = . ;
   __bss_end = . ;
     } > ram
}
