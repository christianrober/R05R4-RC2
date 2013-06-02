cmd_kernel/sys_ni.o := bfin-uclinux-gcc -Wp,-MD,kernel/.sys_ni.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer -fno-builtin -D__linux__ -Wdeclaration-after-statement     -DKBUILD_BASENAME=sys_ni -DKBUILD_MODNAME=sys_ni -c -o kernel/sys_ni.o kernel/sys_ni.c

deps_kernel/sys_ni.o := \
  kernel/sys_ni.c \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \
  include/linux/errno.h \
  include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/asm/unistd.h \

kernel/sys_ni.o: $(deps_kernel/sys_ni.o)

$(deps_kernel/sys_ni.o):
