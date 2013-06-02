cmd_arch/blackfin/lib/muldi3.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/lib/.muldi3.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer -fno-builtin -D__linux__ -Wdeclaration-after-statement     -DKBUILD_BASENAME=muldi3 -DKBUILD_MODNAME=muldi3 -c -o arch/blackfin/lib/muldi3.o arch/blackfin/lib/muldi3.c

deps_arch/blackfin/lib/muldi3.o := \
  arch/blackfin/lib/muldi3.c \

arch/blackfin/lib/muldi3.o: $(deps_arch/blackfin/lib/muldi3.o)

$(deps_arch/blackfin/lib/muldi3.o):
