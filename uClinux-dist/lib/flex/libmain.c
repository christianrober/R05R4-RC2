/* libmain - flex run-time support library "main" function */

/* $Header: /cvsroot/uclinux533/uClinux-dist/lib/flex/libmain.c,v 1.1 2005/08/16 09:09:39 magicyang Exp $ */

extern int yylex();

int main( argc, argv )
int argc;
char *argv[];
	{
	while ( yylex() != 0 )
		;

	return 0;
	}
