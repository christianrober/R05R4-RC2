all:	Makefile
	make install DESTDIR=$(TEMPDIR)

Makefile:
	./configure --host=bfin-uclinux --prefix=/usr --disable-manual --disable-shared --enable-shared=no --enable-static --disable-glib --with-osip=$(TEMPDIR) --with-speex=$(TEMPDIR) LDFLAGS='-Wl,-elf2flt' LIBS='-lm'

clean:
	make clean
	make -i distclean
	
