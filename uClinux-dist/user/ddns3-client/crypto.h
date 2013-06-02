/*
 *	DDNS v3 Client
 *
 *		Author:		Alan Yates <alany@ay.com.au>
 *		Version:	$Id: crypto.h,v 1.1 2005/07/25 01:50:56 magicyang Exp $
 */

#ifndef _CRYPTO_H
#define _CRYPTO_H

char *ddns3_crypto_md5hash(char *s, int len);
char *ddns3_crypto_crypt(char *key, char *salt);

#endif /* _CRYPTO_H */
