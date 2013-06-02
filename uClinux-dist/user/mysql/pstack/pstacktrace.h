/* $Header: /cvsroot/uclinux533/uClinux-dist/user/mysql/pstack/pstacktrace.h,v 1.2 2005/07/25 04:32:42 magicyang Exp $ */

/*
 * Debugging macros.
 */

#ifndef	pstacktrace_h_
#define	pstacktrace_h_

#define	PSTACK_DEBUG 1
#undef PSTACK_DEBUG

#ifdef PSTACK_DEBUG
#	define	TRACE_PUTC(a)		putc a
#	define	TRACE_FPUTS(a)		fputs a
#	define	TRACE_FPRINTF(a)	fprintf a
#else /* PSTACK_DEBUG */
#	define	TRACE_PUTC(a)		(void)0
#	define	TRACE_FPUTS(a)		(void)0
#	define	TRACE_FPRINTF(a)	(void)0
#endif /* !PSTACK_DEBUG */

#endif /* pstacktrace_h_ */

