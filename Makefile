CC=cc
CFLAGS= -O2 -w -lpthread
prefix=/usr/local

all: seed random seedy.o
	ar rcs libseedy.a seedy.o
random: random.c seedy.o mtwist.o
seed: seed.c seedy.o
seedy.o: seedy.c
mtwist.o: mtwist.c

install: all
	mkdir -p $(DESTDIR)$(prefix)/bin 
	mkdir -p $(DESTDIR)$(prefix)/share/man/man1 
	mkdir -p $(DESTDIR)$(prefix)/share/man/man3 
	mkdir -p $(DESTDIR)$(prefix)/include 
	mkdir -p $(DESTDIR)$(prefix)/lib
	install seedy.h $(DESTDIR)$(prefix)/include
	install libseedy.a $(DESTDIR)$(prefix)/lib
	install seedy.3 $(DESTDIR)$(prefix)/share/man/man3
	install seed $(DESTDIR)$(prefix)/bin
	install seed.1 $(DESTDIR)$(prefix)/share/man/man1
	install random $(DESTDIR)$(prefix)/bin
	install random.1 $(DESTDIR)$(prefix)/share/man/man1

uninstall:
	rm -f $(DESTDIR)$(prefix)/bin/seed
	rm -f $(DESTDIR)$(prefix)/include/seed.h
	rm -f $(DESTDIR)$(prefix)/lib/libseed.so
	rm -f $(DESTDIR)$(prefix)/share/man/man1/seed.1

clean:
	rm -f seed random *.o *.so *.a
