CC=cc
CFLAGS= -O2 -w -shared -lpthread
prefix=/usr/local

all: libicm
libicm: libicm.c

install: all
	mkdir -p $(DESTDIR)$(prefix)/lib
	install libicm $(DESTDIR)$(prefix)/lib/libicm.so
	install libicm.h $(DESTDIR)$(prefix)/include/libicm.h

uninstall:
	rm -f $(DESTDIR)$(prefix)/lib/libicm.so
	rm -f $(DESTDIR)$(prefix)/include/libicm.h

clean:
	rm -f libicm *.o
