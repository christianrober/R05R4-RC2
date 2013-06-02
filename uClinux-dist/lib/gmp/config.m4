dnl config.m4.  Generated automatically by configure.
changequote(<,>)
ifdef(<__CONFIG_M4_INCLUDED__>,,<
define(<CONFIG_TOP_SRCDIR>,<`../.'>)
define(<WANT_PROFILING>,<`no'>)
define(<GMP_LIMB_BITS>,(8*4))
define(<GMP_NAIL_BITS>,0)
define(<GMP_NUMB_BITS>,eval(GMP_LIMB_BITS-GMP_NAIL_BITS))
>)
changequote(`,')
ifdef(`__CONFIG_M4_INCLUDED__',,`
include(CONFIG_TOP_SRCDIR`/mpn/asm-defs.m4')
define_not_for_expansion(`HAVE_HOST_CPU_bfin')
define_not_for_expansion(`HAVE_ABI_standard')
')
define(`__CONFIG_M4_INCLUDED__')
