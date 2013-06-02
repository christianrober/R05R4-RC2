cmd_arch/blackfin/mach-common/cplbhdlr.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/mach-common/.cplbhdlr.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__    -c -o arch/blackfin/mach-common/cplbhdlr.o arch/blackfin/mach-common/cplbhdlr.S

deps_arch/blackfin/mach-common/cplbhdlr.o := \
  arch/blackfin/mach-common/cplbhdlr.S \
    $(wildcard include/config/blkfin/cache.h) \
    $(wildcard include/config/blkfin/dcache.h) \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \
  include/asm/cplb.h \
  include/asm/blackfin.h \
  include/asm/macros.h \
  include/asm/mach/blackfin.h \
  include/asm/mach/bf533.h \
    $(wildcard include/config/bf533.h) \
    $(wildcard include/config/bf532.h) \
    $(wildcard include/config/bank/1.h) \
    $(wildcard include/config/bank/0.h) \
    $(wildcard include/config/bank/3.h) \
    $(wildcard include/config/bank/2.h) \
    $(wildcard include/config/c/amben/all.h) \
    $(wildcard include/config/c/amben.h) \
    $(wildcard include/config/c/amben/b0.h) \
    $(wildcard include/config/c/amben/b0/b1.h) \
    $(wildcard include/config/c/amben/b0/b1/b2.h) \
    $(wildcard include/config/c/amcken.h) \
    $(wildcard include/config/c/cdprio.h) \
    $(wildcard include/config/bfin/kernel/clock.h) \
    $(wildcard include/config/vco/mult.h) \
    $(wildcard include/config/clkin/half.h) \
    $(wildcard include/config/vco/hz.h) \
    $(wildcard include/config/clkin/hz.h) \
    $(wildcard include/config/pll/bypass.h) \
    $(wildcard include/config/cclk/hz.h) \
    $(wildcard include/config/cclk/div.h) \
    $(wildcard include/config/sclk/hz.h) \
    $(wildcard include/config/sclk/div.h) \
    $(wildcard include/config/cclk/act/div.h) \
    $(wildcard include/config/cclk/div/not/defined/properly.h) \
    $(wildcard include/config/bf531.h) \
    $(wildcard include/config/mem/size.h) \
    $(wildcard include/config/blkfin/wt.h) \
    $(wildcard include/config/blkfin/wb.h) \
  include/asm/mach/mem_map.h \
  include/asm/mach/defBF532.h \
  include/asm/mach-common/def_LPBlackfin.h \
  include/asm/entry.h \
    $(wildcard include/config/debug/hwerr.h) \
  include/asm/setup.h \
  include/asm/page.h \
  include/asm/page_offset.h \
    $(wildcard include/config/bfin.h) \
  include/asm/io.h \

arch/blackfin/mach-common/cplbhdlr.o: $(deps_arch/blackfin/mach-common/cplbhdlr.o)

$(deps_arch/blackfin/mach-common/cplbhdlr.o):
