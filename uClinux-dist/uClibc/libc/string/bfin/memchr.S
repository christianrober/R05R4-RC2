/* memchr.S
 * Copyright (C) 2003, 2005 Analog Devices Inc., All Rights Reserved.
 *
 * This file is subject to the terms and conditions of the GNU Library General
 * Public License. See the file "COPYING.LIB" in the main directory of this
 * archive for more details.
 *
 * Non-LGPL License also available as part of VisualDSP++
 * http://www.analog.com/processors/resources/crosscore/visualDspDevSoftware.html
 */

	   //===============================================
	   // C Library function MEMCHR
	   // R0 = address
	   // R1 = sought byte
	   // R2 = count
	   // Returns pointer to located character.
	   //

.text;

.align 2;

_memchr:  P0 = R0 ;             // P0 = address
             P2 = R2 ;             // P2 = count
             R1 = R1.B(Z); 
             CC = R2 == 0;
             IF CC JUMP failed;
             		     
bytes:       LSETUP (byte_loop_s , byte_loop_e) LC0=P2;

byte_loop_s:    
	     R3 = B[P0++](Z);
             CC = R3 == R1;		
	     IF CC JUMP found;
	     NOP;
byte_loop_e:    

failed:      R0=0;
             RTS;

found:       R0 = P0;
             R0 += -1;
             RTS;
           .size _memchr, .-_memchr
	   .global _memchr;
	   .type _memchr,STT_FUNC;
