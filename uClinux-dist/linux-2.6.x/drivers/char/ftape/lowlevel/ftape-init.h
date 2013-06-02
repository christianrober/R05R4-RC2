#ifndef _FTAPE_INIT_H
#define _FTAPE_INIT_H

/*
 * Copyright (C) 1993-1996 Bas Laarhoven,
 *           (C) 1996-1997 Claus-Justus Heine.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *
 * $Source: /cvsroot/uclinux533/uClinux-dist/linux-2.6.x/drivers/char/ftape/lowlevel/ftape-init.h,v $
 * $Revision: 1.4 $
 * $Date: 2005/08/12 06:42:25 $
 *
 * This file contains the definitions for the interface to 
 * the Linux kernel for floppy tape driver ftape.
 *
 */

#include <linux/linkage.h>
#include <linux/signal.h>

#define _NEVER_BLOCK    (sigmask(SIGKILL) | sigmask(SIGSTOP))
#define _DONT_BLOCK     (_NEVER_BLOCK | sigmask(SIGINT))
#define _DO_BLOCK       (sigmask(SIGPIPE))

#ifndef QIC117_TAPE_MAJOR
#define QIC117_TAPE_MAJOR 27
#endif

#endif