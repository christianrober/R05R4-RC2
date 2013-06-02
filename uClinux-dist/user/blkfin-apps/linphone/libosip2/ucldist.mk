all:	Makefile
	make install

Makefile:
	./configure --host=bfin-uclinux --prefix=$(TEMPDIR) --disable-shared --enable-static

clean:
	make -i distclean

	
