/* mpfr_set_str_binary -- set a floating-point number from a binary string

Copyright 1999, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

This file is part of the MPFR Library.

The MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPFR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MPFR_NEED_LONGLONG_H
#include "mpfr-impl.h"

/* Currently the number should be of the form +/- xxxx.xxxxxxEyy, with
   decimal exponent. The mantissa of x is supposed to be large enough
   to hold all the bits of str. */

void
mpfr_set_str_binary (mpfr_ptr x, const char *str)
{
  char *str2, *str0, negative = 0;
  unsigned long j, l, k = 0, xsize, cnt, alloc;
  mp_limb_t *xp;
  long expn = 0, e;
  char *endstr2;

  xp = MPFR_MANT(x);
  xsize = MPFR_LIMB_SIZE(x);
  alloc = strlen(str) + 1;

  if (*str == '-')
    {
      negative = 1;
      str++;
    }
  else if (*str == '+')
    str++;

  if (*str == 'N')
    {
      MPFR_SET_NAN(x);
      __gmpfr_flags |= MPFR_FLAGS_NAN;
      return;
    }

  if (*str == 'I')
    {
      MPFR_SET_INF(x);
      if (negative)
	MPFR_SET_NEG(x);
      else
	MPFR_SET_POS(x);
      /*if (MPFR_IS_STRICTNEG(x) != negative)
	MPFR_CHANGE_SIGN(x);*/
      return;
    }

  MPFR_CLEAR_FLAGS(x);

  str0 = str2 = (char *) (*__gmp_allocate_func) (alloc);

  while (*str == '0')
    str++;

  while (*str == '0' || *str == '1')
    {
      *(str2++) = *(str++);
      k++;
    }

  if (*str == '.')
    {
      str++;
      while (*str == '0' || *str == '1')
        {
          *(str2++) = *(str++);
        }
    }

  if (*str == 'e' || *str == 'E')
    {
      char *end;

      e = strtol (++str, &end, 10); /* signed exponent after 'e' or 'E' */
      MPFR_ASSERTN (*end == '\0' && errno != ERANGE);
      expn = k + e;
      MPFR_ASSERTN (expn >= e);
    }
  else
    expn = k;

  endstr2 = str2;
  l = endstr2 - str0; /* length of mantissa */
  if (l == 0)
    { /* input is zero */
      MPFR_SET_ZERO(x);
    }
  else
    {
      l = (l - 1) / BITS_PER_MP_LIMB + 1;
      str2 = str0;

      MPFR_ASSERTN (l <= xsize);

      /* str2[0]..endstr2[-1] contains the mantissa */
      for (k = 1; k <= l; k++)
        {
          j = 0;
          xp[xsize - k] = 0;
          while ((str2 < endstr2) && (j < BITS_PER_MP_LIMB))
            {
              xp[xsize - k] = (xp[xsize - k] << 1) + (*str2 - '0');
              str2++; j++;
            }
          xp[xsize - k] <<= (BITS_PER_MP_LIMB - j);
        }

      for (; k <= xsize; k++)
        xp[xsize - k] = 0;

      count_leading_zeros(cnt, xp[xsize - 1]);
      if (cnt)
        mpn_lshift(xp, xp, xsize, cnt);

      MPFR_SET_EXP (x, expn - cnt);
      if (negative)
	MPFR_SET_NEG(x);
      else
	MPFR_SET_POS(x);
    }

  (*__gmp_free_func) (str0, alloc);

  /* May change to take into account : syntax errors, overflow in exponent,
     string truncated because of size of x */
}
