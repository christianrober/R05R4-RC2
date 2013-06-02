cmd_arch/blackfin/lib/memcmp.o := bfin-uclinux-gcc -Wp,-MD,arch/blackfin/lib/.memcmp.o.d  -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude  -D__ASSEMBLY__    -c -o arch/blackfin/lib/memcmp.o arch/blackfin/lib/memcmp.S

deps_arch/blackfin/lib/memcmp.o := \
  arch/blackfin/lib/memcmp.S \

arch/blackfin/lib/memcmp.o: $(deps_arch/blackfin/lib/memcmp.o)

$(deps_arch/blackfin/lib/memcmp.o):
