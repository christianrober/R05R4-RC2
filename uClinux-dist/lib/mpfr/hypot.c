/* mpfr_hypot -- Euclidean distance

Copyright 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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


#include "mpfr-impl.h"

 /* The computation of hypot of x and y is done by

    hypot(x,y)= sqrt(x^2+y^2) = z
 */

int
mpfr_hypot (mpfr_ptr z, mpfr_srcptr x , mpfr_srcptr y , mp_rnd_t rnd_mode)
{
  int inexact;
  /* Flag exact computation */
  int not_exact;
  mpfr_t t, te, ti; /* auxiliary variables */
  mp_prec_t Nx, Ny, Nz; /* size variables */
  mp_prec_t Nt;   /* precision of the intermediary variable */
  mp_exp_t Ex, Ey, sh;
  mp_exp_unsigned_t diff_exp;

  /* particular cases */
  if (MPFR_ARE_SINGULAR(x,y))
    {
      if (MPFR_IS_NAN(x) || MPFR_IS_NAN(y))
	{
	  MPFR_SET_NAN(z);
	  MPFR_RET_NAN;
	}
      else if (MPFR_IS_INF(x) || MPFR_IS_INF(y))
	{
	  MPFR_SET_INF(z);
	  MPFR_SET_POS(z);
	  MPFR_RET(0);
	}
      else if (MPFR_IS_ZERO(x))
	return mpfr_abs (z, y, rnd_mode);
      else /* y is necessarily 0 */
	return mpfr_abs (z, x, rnd_mode);
    }
  MPFR_CLEAR_FLAGS(z);

  if (mpfr_cmpabs (x, y) < 0)
    {
      mpfr_srcptr t;
      t = x;
      x = y;
      y = t;
    }

  /* now |x| >= |y| */

  Ex = MPFR_GET_EXP (x);
  Ey = MPFR_GET_EXP (y);
  diff_exp = (mp_exp_unsigned_t) Ex - Ey;

  Nz = MPFR_PREC(z);   /* Precision of output variable */

  /* we have x < 2^Ex thus x^2 < 2^(2*Ex),
     and ulp(x) = 2^(Ex-Nx) thus ulp(x^2) >= 2^(2*Ex-2*Nx).
     y does not overlap with the result when
     x^2+y^2 < (|x| + 1/2*ulp(x,Nz))^2 = x^2 + |x|*ulp(x,Nz) + 1/4*ulp(x,Nz)^2,
     i.e. a sufficient condition is y^2 < |x|*ulp(x,Nz),
     or 2^(2*Ey) <= 2^(2*Ex-1-Nz), i.e. 2*diff_exp > Nz.
     Warning: this is true only for Nx <= Nz, otherwise the trailing bits
     of x may be already very close to 1/2*ulp(x,Nz)!
     If Nx > Nz, then we can notice that it is possible to round on Nx bits
     if 2*diff_exp > Nx (see above as if Nz = Nx), therefore on Nz bits.
     Hence the condition: 2*diff_exp > MAX(Nz,Nx).
  */
  if (diff_exp > MAX (Nz, MPFR_PREC (x)) / 2)
    /* result is |x| or |x|+ulp(|x|,Nz) */
    {
      if (rnd_mode == GMP_RNDU)
        {
          /* If z > abs(x), then it was already rounded up; otherwise
             z = abs(x), and we need to add one ulp due to y. */
          if (mpfr_abs (z, x, rnd_mode) == 0)
            mpfr_add_one_ulp (z, rnd_mode);
          return 1;
        }
      else /* GMP_RNDZ, GMP_RNDD, GMP_RNDN */
        {
          inexact = mpfr_abs (z, x, rnd_mode);
          if (inexact == -MPFR_EVEN_INEX)
            {
              /* In case of tie in GMP_RNDN, one must round away from 0.
                 Note: One could do better, by using GMP_RNDNA or similar,
                 but I want to keep the change minimal. Anyway this case
                 should be quite rare in practice. */
              mpfr_add_one_ulp (z, rnd_mode);
              return 1;
            }
          return (inexact) ? inexact : -1;
        }
    }

  /* General case */

  Nx = MPFR_PREC(x);   /* Precision of input variable */
  Ny = MPFR_PREC(y);   /* Precision of input variable */

  /* compute the working precision -- see algorithms.ps */
  Nt = MAX(MAX(MAX(Nx, Ny), Nz), 8);
  Nt = Nt - 8 + __gmpfr_ceil_log2 (Nt);

  /* initialise the intermediary variables */
  mpfr_init (t);
  mpfr_init (te);
  mpfr_init (ti);

  mpfr_save_emin_emax ();

  sh = MAX(0,MIN(Ex,Ey));

  do
    {
      Nt += 10;

      not_exact = 0;
      /* reactualization of the precision */
      mpfr_set_prec (t, Nt);
      mpfr_set_prec (te, Nt);
      mpfr_set_prec (ti, Nt);

      /* computations of hypot */
      mpfr_div_2ui (te, x, sh, GMP_RNDZ); /* exact since Nt >= Nx */
      if (mpfr_mul (te, te, te, GMP_RNDZ))   /* x^2 */
        not_exact = 1;

      mpfr_div_2ui (ti, y, sh, GMP_RNDZ); /* exact since Nt >= Ny */
      if (mpfr_mul (ti, ti, ti, GMP_RNDZ))   /* y^2 */
        not_exact = 1;

      if (mpfr_add (t, te, ti, GMP_RNDZ))  /* x^2+y^2 */
        not_exact = 1;

      if (mpfr_sqrt (t, t, GMP_RNDZ))     /* sqrt(x^2+y^2)*/
        not_exact = 1;

    }
  while (not_exact && !mpfr_can_round (t, Nt - 2, GMP_RNDN, GMP_RNDZ,
                                       Nz + (rnd_mode == GMP_RNDN)));

  inexact = mpfr_mul_2ui (z, t, sh, rnd_mode);
  /* if not_exact=1, necessarily the last (Nt-Nz) bits of t are not all zero,
     otherwise it would not have been possible to round correctly */
  MPFR_ASSERTD(not_exact == 0 || inexact != 0);

  mpfr_clear (t);
  mpfr_clear (ti);
  mpfr_clear (te);
  
  /*
    not_exact  inexact
        0         0         result is exact, ternary flag is 0
        0       non zero    t is exact, ternary flag given by inexact
        1         0         impossible (see above)
        1       non zero    ternary flag given by inexact
   */

  mpfr_restore_emin_emax ();

  return mpfr_check_range (z, inexact, rnd_mode);
}
