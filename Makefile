CC=cc
CFLAGS= -O2 -w -lpthread
prefix=/usr/local

all: src/seed src/random src/genqx64 src/genqx32 src/genqx16 src/genmt32 src/genmt16 src/genss64 src/gensy64 src/prime
	mkdir -p ./bin
	mkdir -p ./lib
	mkdir -p ./include
	ar rcs libseedy.a src/seedy.o src/prime.o src/qx16.o src/qx32.o src/qx64.o src/mt32.o src/mt16.o src/ss64.o 
	cc -Wno-implicit-int -I. -dynamiclib -o libseedy.dylib src/seedy.o src/prime.o src/qx16.o src/qx32.o src/qx64.o src/mt32.o src/mt16.o src/ss64.o 
	cc -o libseedy.so -shared src/seedy.o src/prime.o src/qx16.o src/qx32.o src/qx64.o src/mt32.o src/mt16.o src/ss64.o 
	mv src/genss64 src/gensy64 src/random src/seed src/genqx64 src/genqx32 src/genqx16 src/genmt32 src/genmt16 src/prime bin
	mv *.a *.so *.dylib lib
	cp -rf src/seedy.h include

src/random: src/random.c src/seedy.o src/mt32.o src/ss64.o
src/seed: src/seed.c src/seedy.o

src/gensy64: src/gensy64.c src/seedy.o
src/genss64: src/genss64.c src/ss64.o src/seedy.o
src/genqx64: src/genqx64.c src/qx64.o src/seedy.o
src/genqx32: src/genqx32.c src/qx32.o src/seedy.o
src/genqx16: src/genqx16.c src/qx16.o src/seedy.o
src/genmt32: src/genmt32.c src/mt32.o src/seedy.o
src/genmt16: src/genmt16.c src/mt16.o src/seedy.o
src/prime: src/next_prime.c src/prime.o

src/seedy.o: src/seedy.c 
src/prime.o: src/prime.c
src/qx64.o: src/qx64.c
src/qx32.o: src/qx32.c
src/qx16.o: src/qx16.c
src/mt32.o: src/mt32.c
src/mt16.o: src/mt16.c
src/ss64.o: src/ss64.c

install: all
	mkdir -p $(DESTDIR)$(prefix)/bin 
	mkdir -p $(DESTDIR)$(prefix)/share/man/man1 
	mkdir -p $(DESTDIR)$(prefix)/share/man/man3 
	mkdir -p $(DESTDIR)$(prefix)/include 
	mkdir -p $(DESTDIR)$(prefix)/lib
	install include/* $(DESTDIR)$(prefix)/include
	install lib/* $(DESTDIR)$(prefix)/lib
	install src/man3/* $(DESTDIR)$(prefix)/share/man/man3
	install src/man1/* $(DESTDIR)$(prefix)/share/man/man1
	install bin/* $(DESTDIR)$(prefix)/bin

uninstall:
	rm -f $(DESTDIR)$(prefix)/bin/seed
	rm -f $(DESTDIR)$(prefix)/bin/random
	rm -f $(DESTDIR)$(prefix)/bin/prime
	rm -f $(DESTDIR)$(prefix)/bin/gensy64
	rm -f $(DESTDIR)$(prefix)/bin/genss64
	rm -f $(DESTDIR)$(prefix)/bin/genmt32
	rm -f $(DESTDIR)$(prefix)/bin/genmt16
	rm -f $(DESTDIR)$(prefix)/bin/genqx16
	rm -f $(DESTDIR)$(prefix)/bin/genqx32
	rm -f $(DESTDIR)$(prefix)/bin/genqx64
	rm -f $(DESTDIR)$(prefix)/include/seedy.h
	rm -f $(DESTDIR)$(prefix)/lib/libseedy.*
	rm -f $(DESTDIR)$(prefix)/share/man/man3/seedy.3
	rm -f $(DESTDIR)$(prefix)/share/man/man1/seed.1
	rm -f $(DESTDIR)$(prefix)/share/man/man1/random.1

clean:
	rm -fr bin lib include src/*.o


