#ifndef LINT
static const char rcsid[] = "$Id: setitimer.c,v 1.1 2005/07/22 07:18:02 magicyang Exp $";
#endif

#include "port_before.h"

#include <sys/time.h>

#include "port_after.h"

/*
 * Setitimer emulation routine.
 */
#ifndef NEED_SETITIMER
int __bindcompat_setitimer;
#else

int
__setitimer(int which, const struct itimerval *value,
	    struct itimerval *ovalue)
{
	if (alarm(value->it_value.tv_sec) >= 0)
		return (0);
	else
		return (-1);
}
#endif
