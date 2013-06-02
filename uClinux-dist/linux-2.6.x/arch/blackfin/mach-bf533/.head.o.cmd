cmd_arch/blackfin/mach-bf533/head.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/mach-bf533/.head.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__    -c -o arch/blackfin/mach-bf533/head.o arch/blackfin/mach-bf533/head.S

deps_arch/blackfin/mach-bf533/head.o := \
  arch/blackfin/mach-bf533/head.S \
    $(wildcard include/config/bfin/kernel/clock.h) \
    $(wildcard include/config/debug/kernel/start.h) \
    $(wildcard include/config/vco/mult.h) \
    $(wildcard include/config/cclk/act/div.h) \
    $(wildcard include/config/sclk/div.h) \
    $(wildcard include/config/bfin/shared/flash/enet.h) \
    $(wildcard include/config/baud/9600.h) \
    $(wildcard include/config/baud/19200.h) \
    $(wildcard include/config/baud/38400.h) \
    $(wildcard include/config/baud/57600.h) \
    $(wildcard include/config/baud/115200.h) \
    $(wildcard include/config/clkin/hz.h) \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \
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
    $(wildcard include/config/clkin/half.h) \
    $(wildcard include/config/vco/hz.h) \
    $(wildcard include/config/pll/bypass.h) \
    $(wildcard include/config/cclk/hz.h) \
    $(wildcard include/config/cclk/div.h) \
    $(wildcard include/config/sclk/hz.h) \
    $(wildcard include/config/cclk/div/not/defined/properly.h) \
    $(wildcard include/config/bf531.h) \
    $(wildcard include/config/mem/size.h) \
    $(wildcard include/config/blkfin/wt.h) \
    $(wildcard include/config/blkfin/wb.h) \
  include/asm/mach/mem_map.h \
    $(wildcard include/config/blkfin/cache.h) \
    $(wildcard include/config/blkfin/dcache.h) \
  include/asm/mach/defBF532.h \
  include/asm/mach-common/def_LPBlackfin.h \
  include/asm/mach/mem_init.h \
    $(wildcard include/config/mem/mt48lc16m16a2tg/75.h) \
    $(wildcard include/config/mem/mt48lc64m4a2fb/7e.h) \
    $(wildcard include/config/mem/generic/board.h) \
    $(wildcard include/config/mem/add/width.h) \
    $(wildcard include/config/flash/speed/bwat.h) \
    $(wildcard include/config/flash/speed/brat.h) \
    $(wildcard include/config/flash/speed/bht.h) \
    $(wildcard include/config/flash/speed/bst.h) \
    $(wildcard include/config/flash/speed/btt.h) \
    $(wildcard include/config/flash/speed/rdyen.h) \

arch/blackfin/mach-bf533/head.o: $(deps_arch/blackfin/mach-bf533/head.o)

$(deps_arch/blackfin/mach-bf533/head.o):
