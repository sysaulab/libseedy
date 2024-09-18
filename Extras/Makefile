CC=cc
CFLAGS= -O3 -Wall -lpthread
prefix=/usr/local
src=../src/tools

all: $(src)/gencc832 $(src)/gencc2032 $(src)/genmt32 $(src)/genqx16 $(src)/genqx32 $(src)/genqx64 $(src)/gensy16 $(src)/gensy32 $(src)/gensy64 $(src)/genss64 $(src)/nprime16 $(src)/nprime32 $(src)/nprime64 $(src)/symt32 $(src)/symtqx32 $(src)/symtqx64 $(src)/syss64 $(src)/sycc2032 $(src)/syccqx32 $(src)/syccqx64
	mkdir -p bin
	mv $(src)/gencc832 bin
	mv $(src)/gencc2032 bin
	mv $(src)/genmt32 bin
	mv $(src)/genqx16 bin
	mv $(src)/genqx32 bin
	mv $(src)/genqx64 bin
	mv $(src)/gensy16 bin
	mv $(src)/gensy32 bin
	mv $(src)/gensy64 bin
	mv $(src)/genss64 bin
	mv $(src)/nprime16 bin
	mv $(src)/nprime32 bin
	mv $(src)/nprime64 bin
	mv $(src)/symt32 bin
	mv $(src)/symtqx32 bin
	mv $(src)/symtqx64 bin
	mv $(src)/syss64 bin
	mv $(src)/sycc2032 bin
	mv $(src)/syccqx32 bin
	mv $(src)/syccqx64 bin

$(src)/gencc832: $(src)/gencc832.c
$(src)/gencc2032: $(src)/gencc2032.c
$(src)/genmt32: $(src)/genmt32.c
$(src)/genqx16: $(src)/genqx16.c
$(src)/genqx32: $(src)/genqx32.c
$(src)/genqx64: $(src)/genqx64.c
$(src)/gensy16: $(src)/gensy16.c
$(src)/gensy32: $(src)/gensy32.c
$(src)/gensy64: $(src)/gensy64.c
$(src)/genss64: $(src)/genss64.c
$(src)/nprime64: $(src)/nprime64.c 
$(src)/nprime32: $(src)/nprime32.c 
$(src)/nprime16: $(src)/nprime16.c 
$(src)/symt32: $(src)/symt32.c
$(src)/symtqx32: $(src)/symtqx32.c
$(src)/symtqx64: $(src)/symtqx64.c
$(src)/syss64: $(src)/syss64.c
$(src)/sycc2032: $(src)/sycc2032.c
$(src)/syccqx32: $(src)/syccqx32.c
$(src)/syccqx64: $(src)/syccqx64.c

clean:
	rm -fr bin lib $(src)*.o
	