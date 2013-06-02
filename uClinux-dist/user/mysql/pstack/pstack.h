/* $Header: /cvsroot/uclinux533/uClinux-dist/user/mysql/pstack/pstack.h,v 1.2 2005/07/25 04:32:42 magicyang Exp $ */

#ifndef	pstack_pstack_h_
#define	pstack_pstack_h_

#include	"pstacktrace.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Install the stack-trace-on-SEGV handler....
 */
extern int
pstack_install_segv_action(	const char*	path_format);
#ifdef __cplusplus
}
#endif

#endif /* pstack_pstack_h_ */

