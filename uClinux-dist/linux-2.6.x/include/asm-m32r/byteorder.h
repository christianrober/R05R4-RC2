#ifndef _ASM_M32R_BYTEORDER_H
#define _ASM_M32R_BYTEORDER_H

/* $Id: byteorder.h,v 1.1 2005/08/16 02:52:29 magicyang Exp $ */

#include <asm/types.h>

#if !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#if defined(__LITTLE_ENDIAN__)
#  include <linux/byteorder/little_endian.h>
#else
#  include <linux/byteorder/big_endian.h>
#endif

#endif /* _ASM_M32R_BYTEORDER_H */
