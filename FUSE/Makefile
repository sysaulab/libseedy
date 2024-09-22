src=./
CC=cc
CFLAGS= -O3 -D_FILE_OFFSET_BITS=64 
prefix=/usr/local

all: $(src)/randfs-fuse

$(src)/randfs-fuse: $(src)/randfs-fuse.c /usr/local/lib/libfuse-t.a

clean:
	rm -fr bin include $(src)/randfs-fuse
