/*
 * $Id: keynames.c,v 1.1.1.1 2004/07/19 09:47:25 lgsoft Exp $
 */

#include <test.priv.h>

int main(int argc GCC_UNUSED, char *argv[] GCC_UNUSED)
{
	int n;
	for (n = -1; n < 512; n++) {
		printf("%d(%5o):%s\n", n, n, keyname(n));
	}
	return EXIT_SUCCESS;
}
