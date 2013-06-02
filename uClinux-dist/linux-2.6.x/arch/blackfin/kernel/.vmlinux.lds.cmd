cmd_arch/blackfin/kernel/vmlinux.lds := bfin-uclinux-gcc -E -Wp,-MD,arch/blackfin/kernel/.vmlinux.lds.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude   -P -C -Ublackfin -D__ASSEMBLY__ -o arch/blackfin/kernel/vmlinux.lds arch/blackfin/kernel/vmlinux.lds.S

deps_arch/blackfin/kernel/vmlinux.lds := \
  arch/blackfin/kernel/vmlinux.lds.S \
  include/asm-generic/vmlinux.lds.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/mem_map.h \
  include/asm/mach/mem_map.h \
    $(wildcard include/config/mem/size.h) \
    $(wildcard include/config/bf533.h) \
    $(wildcard include/config/blkfin/cache.h) \
    $(wildcard include/config/blkfin/dcache.h) \
    $(wildcard include/config/bf532.h) \
    $(wildcard include/config/bf531.h) \

arch/blackfin/kernel/vmlinux.lds: $(deps_arch/blackfin/kernel/vmlinux.lds)

$(deps_arch/blackfin/kernel/vmlinux.lds):
