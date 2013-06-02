cmd_arch/blackfin/kernel/entry.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/kernel/.entry.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__    -c -o arch/blackfin/kernel/entry.o arch/blackfin/kernel/entry.S

deps_arch/blackfin/kernel/entry.o := \
  arch/blackfin/kernel/entry.S \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \
  include/asm/thread_info.h \
  include/asm/page.h \
  include/asm/page_offset.h \
    $(wildcard include/config/bfin.h) \
  include/asm/io.h \
  include/asm/entry.h \
    $(wildcard include/config/debug/hwerr.h) \
  include/asm/setup.h \
  include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/asm/asm-offsets.h \

arch/blackfin/kernel/entry.o: $(deps_arch/blackfin/kernel/entry.o)

$(deps_arch/blackfin/kernel/entry.o):
