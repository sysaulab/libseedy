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


clean:
	rm -fr bin include $(src)/seedy
