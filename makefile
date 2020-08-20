.PHONY: all
all: runprod runcons

runprod: prod.c ring_queue.h ipc.h debug.h
	gcc -Wall -O2 -o runprod prod.c -lrt

runcons: cons.c ring_queue.h ipc.h debug.h
	gcc -Wall -O2 -o runcons cons.c -lrt

.PHONY: clean
clean:
	rm runprod runcons
