cmd_arch/blackfin/lib/udivsi3.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/lib/.udivsi3.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__    -c -o arch/blackfin/lib/udivsi3.o arch/blackfin/lib/udivsi3.S

deps_arch/blackfin/lib/udivsi3.o := \
  arch/blackfin/lib/udivsi3.S \

arch/blackfin/lib/udivsi3.o: $(deps_arch/blackfin/lib/udivsi3.o)

$(deps_arch/blackfin/lib/udivsi3.o):
