/*
 * $Id: stub_memaccount.c,v 1.2 2005/07/25 06:29:23 magicyang Exp $
 */

/* Stub function for programs not implementing statMemoryAccounted */
#include "config.h"
#include "util.h"
int
statMemoryAccounted(void)
{
    return -1;
}
