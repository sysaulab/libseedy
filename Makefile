CC=cc
CFLAGS= -O2 -w -lpthread
prefix=/usr/local

all: seed random genqxo64 genqxo32 genqxo16 genmt32 genmt16 prime
	mkdir -p ./bin
	mkdir -p ./lib
	mkdir -p ./include
	ar rcs libseedy.a seedy.o prime.o qxo16.o qxo32.o qxo64.o mt32.o mt16.o ss64.o 
	cc -Wno-implicit-int -I. -dynamiclib -o libseedy.dylib seedy.o prime.o qxo16.o qxo32.o qxo64.o mt32.o mt16.o ss64.o 
	cc -o seedo.so -shared seedy.o prime.o qxo16.o qxo32.o qxo64.o mt32.o mt16.o ss64.o 
	mv random seed genqxo64 genqxo32 genqxo16 genmt32 genmt16 prime bin
	mv *.a *.so *.dylib lib
	cp -rf seedy.h shishua include
	rm *.o

random: random.c seedy.o mt32.o ss64.o
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
ss64.o: ss64.c

install: all
	mkdir -p $(DESTDIR)$(prefix)/bin 
	mkdir -p $(DESTDIR)$(prefix)/share/man/man1 
	mkdir -p $(DESTDIR)$(prefix)/share/man/man3 
	mkdir -p $(DESTDIR)$(prefix)/include 
	mkdir -p $(DESTDIR)$(prefix)/lib
	install include/* $(DESTDIR)$(prefix)/include
	install lib/* $(DESTDIR)$(prefix)/lib
	install man3/* $(DESTDIR)$(prefix)/share/man/man3
	install man1/* $(DESTDIR)$(prefix)/share/man/man1
	install bin/* $(DESTDIR)$(prefix)/bin

uninstall:
	rm -f $(DESTDIR)$(prefix)/bin/seed
	rm -f $(DESTDIR)$(prefix)/include/seed.h
	rm -f $(DESTDIR)$(prefix)/lib/libseed.so
	rm -f $(DESTDIR)$(prefix)/share/man/man1/seed.1

clean:
	rm -fr bin lib include





