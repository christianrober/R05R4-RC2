cmd_arch/blackfin/lib/outs.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/lib/.outs.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__    -c -o arch/blackfin/lib/outs.o arch/blackfin/lib/outs.S

deps_arch/blackfin/lib/outs.o := \
  arch/blackfin/lib/outs.S \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \

arch/blackfin/lib/outs.o: $(deps_arch/blackfin/lib/outs.o)

$(deps_arch/blackfin/lib/outs.o):
