cmd_arch/blackfin/lib/lshrdi3.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/lib/.lshrdi3.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer -fno-builtin -D__linux__ -Wdeclaration-after-statement     -DKBUILD_BASENAME=lshrdi3 -DKBUILD_MODNAME=lshrdi3 -c -o arch/blackfin/lib/lshrdi3.o arch/blackfin/lib/lshrdi3.c

deps_arch/blackfin/lib/lshrdi3.o := \
  arch/blackfin/lib/lshrdi3.c \

arch/blackfin/lib/lshrdi3.o: $(deps_arch/blackfin/lib/lshrdi3.o)

$(deps_arch/blackfin/lib/lshrdi3.o):
