cmd_.tmp_kallsyms2.o := bfin-uclinux-gcc -Wp,-MD,./..tmp_kallsyms2.o.d -D__ASSEMBLY__   -nostdinc -isystem /opt/uClinux/bfin-uclinux/lib/gcc/bfin-uclinux/3.4.4/include -D__KERNEL__ -Iinclude     -c -o .tmp_kallsyms2.o .tmp_kallsyms2.S

deps_.tmp_kallsyms2.o := \
  .tmp_kallsyms2.S \
  include/asm/types.h \

.tmp_kallsyms2.o: $(deps_.tmp_kallsyms2.o)

$(deps_.tmp_kallsyms2.o):
