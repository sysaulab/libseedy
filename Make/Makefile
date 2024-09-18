src=../src
CC=cc
CFLAGS= -O3 -lpthread -I$(src)
prefix=/usr/local

all: $(src)/seedy
	mkdir -p ./bin
	mkdir -p ./include/libseedy
	cp -rf $(src)/libseedy include
	cp -f $(src)/seedy bin

$(src)/seedy: $(src)/seedy.c

install: all
	mkdir -p $(DESTDIR)$(prefix)/include/libseedy
	mkdir -p $(DESTDIR)$(prefix)/bin
	install bin/* $(DESTDIR)$(prefix)/bin
	install include/libseedy/* $(DESTDIR)$(prefix)/include/libseedy

uninstall:
	rm -f $(DESTDIR)$(prefix)/include/libseedy/*
	rm -fr $(DESTDIR)$(prefix)/include/libseedy
	rm -f $(DESTDIR)$(prefix)/bin/seedy

clean:
	rm -fr bin include $(src)/seedy
