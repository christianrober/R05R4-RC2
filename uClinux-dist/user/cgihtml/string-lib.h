/* string-lib.h - headers for string-lib.c
   $Id: string-lib.h,v 1.2 2005/07/25 01:49:20 magicyang Exp $
*/

char *newstr(char *str);
char *substr(char *str, int offset, int len);
char *replace_ltgt(char *str);
char *lower_case(char *buffer);
