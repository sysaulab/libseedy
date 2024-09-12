CC=cc
CFLAGS= -O2 -w -lpthread
prefix=/usr/local

all: seed random genqxo64 genqxo32 genqxo16 genmt32 prime
	ar rcs libseedy.a seedy.o
random: random.c seedy.o mt32.o
seed: seed.c seedy.o
genqxo64: genqxo64.c qxo64.o prime.o seedy.o
genqxo32: genqxo32.c qxo32.o prime.o seedy.o
genqxo16: genqxo16.c qxo16.o prime.o seedy.o
genmt32: genmt32.c mt32.o prime.o seedy.o
genmt16: genmt16.c mt16.o prime.o seedy.o
prime: next_prime.c prime.o
seedy.o: seedy.c
prime.o: prime.c
qxo64.o: qxo64.c
qxo32.o: qxo32.c
qxo16.o: qxo16.c
mt32.o: mt32.c
mt16.o: mt16.c

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
