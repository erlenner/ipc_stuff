
IPC_DIR=../..

.PHONY: all
all: runprod runcons

runprod: prod.c ${IPC_DIR}/ring_queue.h ${IPC_DIR}/ipc.h ${IPC_DIR}/debug.h
	gcc -Wall -O2 -I${IPC_DIR} -o runprod prod.c -lrt

runcons: cons.c ${IPC_DIR}/ring_queue.h ${IPC_DIR}/ipc.h ${IPC_DIR}/debug.h
	gcc -Wall -O2 -I${IPC_DIR} -o runcons cons.c -lrt

.PHONY: clean
clean:
	rm runprod runcons
