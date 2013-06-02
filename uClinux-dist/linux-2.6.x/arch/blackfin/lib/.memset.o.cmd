cmd_arch/blackfin/lib/memset.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/lib/.memset.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__    -c -o arch/blackfin/lib/memset.o arch/blackfin/lib/memset.S

deps_arch/blackfin/lib/memset.o := \
  arch/blackfin/lib/memset.S \

arch/blackfin/lib/memset.o: $(deps_arch/blackfin/lib/memset.o)

$(deps_arch/blackfin/lib/memset.o):
