all:	Makefile
	make install

Makefile:
	./configure --host=bfin-uclinux --prefix=$(TEMPDIR) --disable-oggtest --enable-fixed-point --enable-blackfin-asm --without-ogg

clean:
	make -i distclean
	
