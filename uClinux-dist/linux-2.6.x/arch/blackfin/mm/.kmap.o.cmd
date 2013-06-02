cmd_arch/blackfin/mm/kmap.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/mm/.kmap.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -Wall -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer -fno-builtin -D__linux__ -Wdeclaration-after-statement     -DKBUILD_BASENAME=kmap -DKBUILD_MODNAME=kmap -c -o arch/blackfin/mm/kmap.o arch/blackfin/mm/kmap.c

deps_arch/blackfin/mm/kmap.o := \
  arch/blackfin/mm/kmap.c \

arch/blackfin/mm/kmap.o: $(deps_arch/blackfin/mm/kmap.o)

$(deps_arch/blackfin/mm/kmap.o):
