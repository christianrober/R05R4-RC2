/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.12.1-BFIN-2005R4
 * Fri Dec  5 15:21:21 2008
 */
#define AUTOCONF_INCLUDED
#undef CONFIG_MMU
#undef CONFIG_FPU
#define CONFIG_UID16 1
#define CONFIG_RWSEM_GENERIC_SPINLOCK 1
#undef CONFIG_RWSEM_XCHGADD_ALGORITHM
#define CONFIG_BFIN 1
#define CONFIG_UCLINUX 1
#define CONFIG_FORCE_MAX_ZONEORDER 14

/*
 * Code maturity level options
 */
#define CONFIG_EXPERIMENTAL 1
#define CONFIG_CLEAN_COMPILE 1
#define CONFIG_BROKEN_ON_SMP 1
#define CONFIG_INIT_ENV_ARG_LIMIT 32

/*
 * General setup
 */
#define CONFIG_LOCALVERSION ""
#undef CONFIG_POSIX_MQUEUE
#undef CONFIG_BSD_PROCESS_ACCT
#undef CONFIG_SYSCTL
#undef CONFIG_AUDIT
#define CONFIG_LOG_BUF_SHIFT 14
#define CONFIG_GENERIC_CALIBRATE_DELAY 1
#undef CONFIG_HOTPLUG
#define CONFIG_KOBJECT_UEVENT 1
#undef CONFIG_IKCONFIG
#define CONFIG_EMBEDDED 1
#define CONFIG_KALLSYMS 1
#undef CONFIG_KALLSYMS_EXTRA_PASS
#define CONFIG_PRINTK 1
#define CONFIG_BUG 1
#define CONFIG_BASE_FULL 1
#define CONFIG_FUTEX 1
#define CONFIG_EPOLL 1
#undef CONFIG_CC_OPTIMIZE_FOR_SIZE
#define CONFIG_CC_ALIGN_FUNCTIONS 0
#define CONFIG_CC_ALIGN_LABELS 0
#define CONFIG_CC_ALIGN_LOOPS 0
#define CONFIG_CC_ALIGN_JUMPS 0
#define CONFIG_BASE_SMALL 0

/*
 * Loadable module support
 */
#define CONFIG_MODULES 1
#define CONFIG_MODULE_UNLOAD 1
#define CONFIG_MODULE_FORCE_UNLOAD 1
#define CONFIG_OBSOLETE_MODPARM 1
#undef CONFIG_MODVERSIONS
#undef CONFIG_MODULE_SRCVERSION_ALL
#undef CONFIG_KMOD

/*
 * Blackfin Processor Options
 */

/*
 * Processor and Board Settings
 */
#define CONFIG_BF533 1
#undef CONFIG_BF532
#undef CONFIG_BF531
#undef CONFIG_BF534
#undef CONFIG_BF535
#undef CONFIG_BF536
#undef CONFIG_BF537
#undef CONFIG_BF561
#define CONFIG_BLACKFIN 1
#define CONFIG_BFIN_SINGLE_CORE 1
#undef CONFIG_BFIN533_EZKIT
#define CONFIG_BFIN533_STAMP 1
#undef CONFIG_BFIN537_STAMP
#undef CONFIG_BFIN533_BLUETECHNIX_CM
#undef CONFIG_BFIN561_EZKIT
#undef CONFIG_GENERIC_BOARD
#define CONFIG_MEM_MT48LC64M4A2FB_7E 1
#define CONFIG_BFIN_SHARED_FLASH_ENET 1

/*
 * BF533/2/1 Specific Configuration
 */

/*
 * Interrupt Priority Assignment
 */

/*
 * Priority
 */
#define CONFIG_UART_ERROR 7
#define CONFIG_SPORT0_ERROR 7
#define CONFIG_SPI_ERROR 7
#define CONFIG_SPORT1_ERROR 7
#define CONFIG_PPI_ERROR 7
#define CONFIG_DMA_ERROR 7
#define CONFIG_PLLWAKE_ERROR 7
#define CONFIG_RTC_ERROR 8
#define CONFIG_DMA0_PPI 8
#define CONFIG_DMA1_SPORT0RX 9
#define CONFIG_DMA2_SPORT0TX 9
#define CONFIG_DMA3_SPORT1RX 9
#define CONFIG_DMA4_SPORT1TX 9
#define CONFIG_DMA5_SPI 10
#define CONFIG_DMA6_UARTRX 10
#define CONFIG_DMA7_UARTTX 10
#define CONFIG_TIMER0 11
#define CONFIG_TIMER1 11
#define CONFIG_TIMER2 11
#define CONFIG_PFA 12
#define CONFIG_PFB 12
#define CONFIG_MEMDMA0 13
#define CONFIG_MEMDMA1 13
#define CONFIG_WDTIMER 13
#undef CONFIG_BFIN_HAVE_RTC

/*
 * Board specific issues
 */

/*
 * Board Setup
 */
#define CONFIG_CLKIN_HZ 11059200
#define CONFIG_MEM_SIZE 128
#define CONFIG_MEM_ADD_WIDTH 11
#define CONFIG_ENET_FLASH_PIN 1

/*
 * LED Status Indicators
 */
#undef CONFIG_BFIN_ALIVE_LED
#undef CONFIG_BFIN_IDLE_LED
#define CONFIG_BFIN_ALIVE_LED_PORT 0xFFC00700
#define CONFIG_BFIN_ALIVE_LED_DPORT 0xFFC00730
#define CONFIG_BFIN_IDLE_LED_PORT 0xFFC00700
#define CONFIG_BFIN_IDLE_LED_DPORT 0xFFC00730

/*
 * Console UART Setup
 */
#undef CONFIG_BAUD_9600
#undef CONFIG_BAUD_19200
#undef CONFIG_BAUD_38400
#define CONFIG_BAUD_57600 1
#undef CONFIG_BAUD_115200
#define CONFIG_BAUD_NO_PARITY 1
#undef CONFIG_BAUD_PARITY
#define CONFIG_BAUD_1_STOPBIT 1
#undef CONFIG_BAUD_2_STOPBIT
#define CONFIG_RAMKERNEL 1
#undef CONFIG_ROMKERNEL
#define CONFIG_LARGE_ALLOCS 1
#define CONFIG_IRQCHIP_DEMUX_GPIO 1

/*
 * DMA Support
 */
#undef CONFIG_NO_DMA
#define CONFIG_BFIN_DMA_5XX 1

/*
 * Cache Support
 */
#define CONFIG_BLKFIN_CACHE 1
#define CONFIG_BLKFIN_DCACHE 1
#undef CONFIG_BLKFIN_CACHE_LOCK
#undef CONFIG_BLKFIN_WB
#define CONFIG_BLKFIN_WT 1
#define CONFIG_UNCACHED_1M 1

/*
 * Clock Settings
 */
#define CONFIG_BFIN_KERNEL_CLOCK 1
#define CONFIG_VCO_MULT 43
#define CONFIG_CCLK_DIV 1
#define CONFIG_SCLK_DIV 6
#undef CONFIG_CLKIN_HALF
#undef CONFIG_PLL_BYPASS

/*
 * Asynchonous Memory Configuration
 */

/*
 * EBIU_AMBCTL Global Control
 */
#define CONFIG_C_AMCKEN 1
#define CONFIG_C_CDPRIO 1
#undef CONFIG_C_AMBEN
#undef CONFIG_C_AMBEN_B0
#undef CONFIG_C_AMBEN_B0_B1
#undef CONFIG_C_AMBEN_B0_B1_B2
#define CONFIG_C_AMBEN_ALL 1

/*
 * EBIU_AMBCTL Control
 */
#define CONFIG_BANK_0 0x7BB0
#define CONFIG_BANK_1 0x7BB0
#define CONFIG_BANK_2 0x7BB0
#define CONFIG_BANK_3 0x99B3
#undef CONFIG_BFIN_SPORT

/*
 * Bus options (PCI, PCMCIA, EISA, MCA, ISA)
 */
#undef CONFIG_PCI

/*
 * PCCARD (PCMCIA/CardBus) support
 */
#undef CONFIG_PCCARD

/*
 * PCI Hotplug Support
 */

/*
 * Executable File Formats
 */
#define CONFIG_BINFMT_FLAT 1
#undef CONFIG_BINFMT_ZFLAT
#undef CONFIG_BINFMT_SHARED_FLAT
#undef CONFIG_BINFMT_MISC

/*
 * Power management options
 */
#undef CONFIG_PM

/*
 * CPU Frequency scaling
 */
#undef CONFIG_CPU_FREQ

/*
 * Generic Driver Options
 */
#define CONFIG_STANDALONE 1
#define CONFIG_PREVENT_FIRMWARE_BUILD 1
#undef CONFIG_FW_LOADER

/*
 * Memory Technology Devices (MTD)
 */
#define CONFIG_MTD 1
#define CONFIG_MTD_DEBUG 1
#define CONFIG_MTD_DEBUG_VERBOSE 0
#undef CONFIG_MTD_CONCAT
#define CONFIG_MTD_PARTITIONS 1
#undef CONFIG_MTD_REDBOOT_PARTS
#define CONFIG_MTD_CMDLINE_PARTS 1

/*
 * User Modules And Translation Layers
 */
#undef CONFIG_MTD_CHAR
#define CONFIG_MTD_BLOCK 1
#undef CONFIG_FTL
#undef CONFIG_NFTL
#undef CONFIG_INFTL

/*
 * RAM/ROM/Flash chip drivers
 */
#undef CONFIG_MTD_CFI
#define CONFIG_MTD_JEDECPROBE 1
#define CONFIG_MTD_GEN_PROBE 1
#undef CONFIG_MTD_CFI_ADV_OPTIONS
#define CONFIG_MTD_MAP_BANK_WIDTH_1 1
#define CONFIG_MTD_MAP_BANK_WIDTH_2 1
#define CONFIG_MTD_MAP_BANK_WIDTH_4 1
#undef CONFIG_MTD_MAP_BANK_WIDTH_8
#undef CONFIG_MTD_MAP_BANK_WIDTH_16
#undef CONFIG_MTD_MAP_BANK_WIDTH_32
#define CONFIG_MTD_CFI_I1 1
#define CONFIG_MTD_CFI_I2 1
#undef CONFIG_MTD_CFI_I4
#undef CONFIG_MTD_CFI_I8
#undef CONFIG_MTD_CFI_INTELEXT
#undef CONFIG_MTD_CFI_AMDSTD
#undef CONFIG_MTD_CFI_STAA
#define CONFIG_MTD_MW320D 1
#undef CONFIG_MTD_M25P64
#define CONFIG_MTD_RAM 1
#undef CONFIG_MTD_ROM
#undef CONFIG_MTD_ABSENT

/*
 * Mapping drivers for chip access
 */
#define CONFIG_MTD_COMPLEX_MAPPINGS 1
#undef CONFIG_MTD_BF5xx_SPI
#define CONFIG_MTD_BF5xx 1
#define CONFIG_BFIN_FLASH_SIZE 0x400000
#define CONFIG_EBIU_FLASH_BASE 0x20000000

/*
 * FLASH_EBIU_AMBCTL Control
 */
#define CONFIG_BFIN_FLASH_BANK_0 0x7BB0
#define CONFIG_BFIN_FLASH_BANK_1 0x7BB0
#define CONFIG_BFIN_FLASH_BANK_2 0x7BB0
#define CONFIG_BFIN_FLASH_BANK_3 0x7BB0
#define CONFIG_MTD_UCLINUX 1

/*
 * Self-contained MTD device drivers
 */
#undef CONFIG_MTD_SLRAM
#undef CONFIG_MTD_PHRAM
#undef CONFIG_MTD_MTDRAM
#undef CONFIG_MTD_BLKMTD
#undef CONFIG_MTD_BLOCK2MTD

/*
 * Disk-On-Chip Device Drivers
 */
#undef CONFIG_MTD_DOC2000
#undef CONFIG_MTD_DOC2001
#undef CONFIG_MTD_DOC2001PLUS

/*
 * NAND Flash Device Drivers
 */
#undef CONFIG_MTD_NAND

/*
 * Parallel port support
 */
#undef CONFIG_PARPORT

/*
 * Plug and Play support
 */

/*
 * Block devices
 */
#undef CONFIG_BLK_DEV_FD
#undef CONFIG_BLK_DEV_COW_COMMON
#undef CONFIG_BLK_DEV_LOOP
#undef CONFIG_BLK_DEV_NBD
#define CONFIG_BLK_DEV_RAM 1
#define CONFIG_BLK_DEV_RAM_COUNT 16
#define CONFIG_BLK_DEV_RAM_SIZE 4096
#undef CONFIG_BLK_DEV_INITRD
#define CONFIG_INITRAMFS_SOURCE ""
#undef CONFIG_CDROM_PKTCDVD

/*
 * IO Schedulers
 */
#define CONFIG_IOSCHED_NOOP 1
#undef CONFIG_IOSCHED_AS
#undef CONFIG_IOSCHED_DEADLINE
#define CONFIG_IOSCHED_CFQ 1
#undef CONFIG_ATA_OVER_ETH

/*
 * ATA/ATAPI/MFM/RLL support
 */
#undef CONFIG_IDE

/*
 * IDE Extra configuration
 */

/*
 * SCSI device support
 */
#undef CONFIG_SCSI

/*
 * Multi-device support (RAID and LVM)
 */
#undef CONFIG_MD

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
#define CONFIG_NET 1

/*
 * Networking options
 */
#define CONFIG_PACKET 1
#undef CONFIG_PACKET_MMAP
#define CONFIG_UNIX 1
#undef CONFIG_NET_KEY
#define CONFIG_INET 1
#undef CONFIG_IP_MULTICAST
#undef CONFIG_IP_ADVANCED_ROUTER
#undef CONFIG_IP_PNP
#undef CONFIG_NET_IPIP
#undef CONFIG_NET_IPGRE
#undef CONFIG_ARPD
#define CONFIG_SYN_COOKIES 1
#undef CONFIG_INET_AH
#undef CONFIG_INET_ESP
#undef CONFIG_INET_IPCOMP
#undef CONFIG_INET_TUNNEL
#define CONFIG_IP_TCPDIAG 1
#undef CONFIG_IP_TCPDIAG_IPV6
#undef CONFIG_IPV6
#undef CONFIG_NETFILTER

/*
 * SCTP Configuration (EXPERIMENTAL)
 */
#undef CONFIG_IP_SCTP
#undef CONFIG_ATM
#undef CONFIG_BRIDGE
#undef CONFIG_VLAN_8021Q
#undef CONFIG_DECNET
#undef CONFIG_LLC2
#undef CONFIG_IPX
#undef CONFIG_ATALK
#undef CONFIG_X25
#undef CONFIG_LAPB
#undef CONFIG_NET_DIVERT
#undef CONFIG_ECONET
#undef CONFIG_WAN_ROUTER

/*
 * QoS and/or fair queueing
 */
#undef CONFIG_NET_SCHED
#undef CONFIG_NET_CLS_ROUTE

/*
 * Network testing
 */
#undef CONFIG_NET_PKTGEN
#undef CONFIG_NETPOLL
#undef CONFIG_NET_POLL_CONTROLLER
#undef CONFIG_HAMRADIO
#undef CONFIG_IRDA
#undef CONFIG_BT
#define CONFIG_NETDEVICES 1
#undef CONFIG_DUMMY
#undef CONFIG_BONDING
#undef CONFIG_EQUALIZER
#undef CONFIG_TUN

/*
 * Ethernet (10 or 100Mbit)
 */
#define CONFIG_NET_ETHERNET 1
#define CONFIG_MII 1
#define CONFIG_SMC91X 1

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
#undef CONFIG_NET_RADIO

/*
 * Wan interfaces
 */
#undef CONFIG_WAN
#undef CONFIG_PPP
#undef CONFIG_SLIP
#undef CONFIG_SHAPER
#undef CONFIG_NETCONSOLE

/*
 * ISDN subsystem
 */
#undef CONFIG_ISDN

/*
 * Telephony Support
 */
#undef CONFIG_PHONE

/*
 * Input device support
 */
#undef CONFIG_INPUT

/*
 * Hardware I/O ports
 */
#undef CONFIG_SERIO
#undef CONFIG_GAMEPORT

/*
 * I2C support
 */
#undef CONFIG_I2C

/*
 * Character devices
 */
#undef CONFIG_BFIN_SPI_CHANNEL
#undef CONFIG_SPI_ADC_BF533
#undef CONFIG_BF533_PFLAGS
#undef CONFIG_BF5xx_PPIFCD
#undef CONFIG_BF5xx_PPI
#undef CONFIG_BFIN_TIMER_LATENCY
#undef CONFIG_VT
#undef CONFIG_SERIAL_NONSTANDARD

/*
 * Serial drivers
 */
#undef CONFIG_SERIAL_8250

/*
 * Non-8250 serial port support
 */
#define CONFIG_SERIAL_BLACKFIN 1
#define CONFIG_SERIAL_BLACKFIN_DMA 1
#undef CONFIG_SERIAL_BLACKFIN_PIO
#define CONFIG_UNIX98_PTYS 1
#define CONFIG_LEGACY_PTYS 1
#define CONFIG_LEGACY_PTY_COUNT 256

/*
 * IPMI
 */
#undef CONFIG_IPMI_HANDLER

/*
 * Watchdog Cards
 */
#undef CONFIG_WATCHDOG
#undef CONFIG_RTC
#undef CONFIG_GEN_RTC
#undef CONFIG_BLACKFIN_RTC
#define CONFIG_BLACKFIN_DPMC 1
#undef CONFIG_DTLK
#undef CONFIG_R3964

/*
 * Ftape, the floppy tape device driver
 */
#undef CONFIG_DRM
#undef CONFIG_RAW_DRIVER

/*
 * TPM devices
 */

/*
 * Multimedia devices
 */
#undef CONFIG_VIDEO_DEV

/*
 * Digital Video Broadcasting Devices
 */
#undef CONFIG_DVB

/*
 * File systems
 */
#define CONFIG_EXT2_FS 1
#define CONFIG_EXT2_FS_XATTR 1
#undef CONFIG_EXT2_FS_POSIX_ACL
#undef CONFIG_EXT2_FS_SECURITY
#undef CONFIG_EXT3_FS
#undef CONFIG_JBD
#define CONFIG_FS_MBCACHE 1
#undef CONFIG_REISERFS_FS
#undef CONFIG_JFS_FS

/*
 * XFS support
 */
#undef CONFIG_XFS_FS
#undef CONFIG_MINIX_FS
#undef CONFIG_ROMFS_FS
#undef CONFIG_QUOTA
#define CONFIG_DNOTIFY 1
#undef CONFIG_AUTOFS_FS
#undef CONFIG_AUTOFS4_FS

/*
 * CD-ROM/DVD Filesystems
 */
#undef CONFIG_ISO9660_FS
#undef CONFIG_UDF_FS

/*
 * DOS/FAT/NT Filesystems
 */
#define CONFIG_FAT_FS 1
#undef CONFIG_MSDOS_FS
#define CONFIG_VFAT_FS 1
#define CONFIG_FAT_DEFAULT_CODEPAGE 437
#define CONFIG_FAT_DEFAULT_IOCHARSET "iso8859-1"
#undef CONFIG_NTFS_FS

/*
 * Pseudo filesystems
 */
#define CONFIG_PROC_FS 1
#define CONFIG_SYSFS 1
#undef CONFIG_DEVFS_FS
#undef CONFIG_DEVPTS_FS_XATTR
#undef CONFIG_TMPFS
#undef CONFIG_HUGETLB_PAGE
#define CONFIG_RAMFS 1

/*
 * Miscellaneous filesystems
 */
#undef CONFIG_ADFS_FS
#undef CONFIG_AFFS_FS
#undef CONFIG_HFS_FS
#undef CONFIG_HFSPLUS_FS
#undef CONFIG_BEFS_FS
#undef CONFIG_BFS_FS
#undef CONFIG_EFS_FS
#undef CONFIG_YAFFS_FS
#undef CONFIG_JFFS_FS
#define CONFIG_JFFS2_FS 1
#define CONFIG_JFFS2_FS_DEBUG 0
#undef CONFIG_JFFS2_FS_NAND
#undef CONFIG_JFFS2_FS_NOR_ECC
#undef CONFIG_JFFS2_COMPRESSION_OPTIONS
#define CONFIG_JFFS2_ZLIB 1
#define CONFIG_JFFS2_RTIME 1
#undef CONFIG_JFFS2_RUBIN
#undef CONFIG_CRAMFS
#undef CONFIG_VXFS_FS
#undef CONFIG_HPFS_FS
#undef CONFIG_QNX4FS_FS
#undef CONFIG_SYSV_FS
#undef CONFIG_UFS_FS

/*
 * Network File Systems
 */
#undef CONFIG_NFS_FS
#undef CONFIG_NFSD
#undef CONFIG_SMB_FS
#undef CONFIG_CIFS
#undef CONFIG_NCP_FS
#undef CONFIG_CODA_FS
#undef CONFIG_AFS_FS

/*
 * Partition Types
 */
#undef CONFIG_PARTITION_ADVANCED
#define CONFIG_MSDOS_PARTITION 1

/*
 * Native Language Support
 */
#define CONFIG_NLS 1
#define CONFIG_NLS_DEFAULT "iso8859-1"
#define CONFIG_NLS_CODEPAGE_437 1
#undef CONFIG_NLS_CODEPAGE_737
#undef CONFIG_NLS_CODEPAGE_775
#undef CONFIG_NLS_CODEPAGE_850
#undef CONFIG_NLS_CODEPAGE_852
#undef CONFIG_NLS_CODEPAGE_855
#undef CONFIG_NLS_CODEPAGE_857
#undef CONFIG_NLS_CODEPAGE_860
#undef CONFIG_NLS_CODEPAGE_861
#undef CONFIG_NLS_CODEPAGE_862
#undef CONFIG_NLS_CODEPAGE_863
#undef CONFIG_NLS_CODEPAGE_864
#undef CONFIG_NLS_CODEPAGE_865
#undef CONFIG_NLS_CODEPAGE_866
#undef CONFIG_NLS_CODEPAGE_869
#undef CONFIG_NLS_CODEPAGE_936
#undef CONFIG_NLS_CODEPAGE_950
#undef CONFIG_NLS_CODEPAGE_932
#undef CONFIG_NLS_CODEPAGE_949
#undef CONFIG_NLS_CODEPAGE_874
#undef CONFIG_NLS_ISO8859_8
#undef CONFIG_NLS_CODEPAGE_1250
#undef CONFIG_NLS_CODEPAGE_1251
#undef CONFIG_NLS_ASCII
#define CONFIG_NLS_ISO8859_1 1
#undef CONFIG_NLS_ISO8859_2
#undef CONFIG_NLS_ISO8859_3
#undef CONFIG_NLS_ISO8859_4
#undef CONFIG_NLS_ISO8859_5
#undef CONFIG_NLS_ISO8859_6
#undef CONFIG_NLS_ISO8859_7
#undef CONFIG_NLS_ISO8859_9
#undef CONFIG_NLS_ISO8859_13
#undef CONFIG_NLS_ISO8859_14
#undef CONFIG_NLS_ISO8859_15
#undef CONFIG_NLS_KOI8_R
#undef CONFIG_NLS_KOI8_U
#undef CONFIG_NLS_UTF8

/*
 * Graphics support
 */
#undef CONFIG_FB

/*
 * Sound
 */
#undef CONFIG_SOUND

/*
 * USB support
 */
#undef CONFIG_USB_ARCH_HAS_HCD
#undef CONFIG_USB_ARCH_HAS_OHCI

/*
 * USB Gadget Support
 */
#undef CONFIG_USB_GADGET

/*
 * Profiling support
 */
#undef CONFIG_PROFILING

/*
 * Kernel hacking
 */
#undef CONFIG_DEBUG_INFO
#undef CONFIG_DEBUG_KERNEL
#undef CONFIG_DEBUG_HWERR
#undef CONFIG_FRAME_POINTER
#undef CONFIG_MAGIC_SYSRQ
#define CONFIG_BOOTPARAM 1
#define CONFIG_BOOTPARAM_STRING "root=/dev/mtdblock3 rw"
#undef CONFIG_NO_KERNEL_MSG
#define CONFIG_CPLB_INFO 1

/*
 * Security options
 */
#undef CONFIG_KEYS
#define CONFIG_SECURITY 1
#undef CONFIG_SECURITY_NETWORK
#define CONFIG_SECURITY_CAPABILITIES 1
#undef CONFIG_SECURITY_SECLVL
#undef CONFIG_SECURITY_SELINUX

/*
 * Cryptographic options
 */
#undef CONFIG_CRYPTO

/*
 * Hardware crypto devices
 */

/*
 * Library routines
 */
#undef CONFIG_CRC_CCITT
#define CONFIG_CRC32 1
#undef CONFIG_LIBCRC32C
#define CONFIG_ZLIB_INFLATE 1
#define CONFIG_ZLIB_DEFLATE 1
