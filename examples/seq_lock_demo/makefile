
IPC_DIR=../../include

.PHONY: all
all: runprod runcons

runprod: prod.c ${IPC_DIR}/seq_lock.h ${IPC_DIR}/ipc.h ${IPC_DIR}/debug.h
	gcc -Wall -O2 -Wall -I${IPC_DIR} -o runprod prod.c -lrt

runcons: cons.c ${IPC_DIR}/seq_lock.h ${IPC_DIR}/ipc.h ${IPC_DIR}/debug.h
	gcc -Wall -O2 -Wall -I${IPC_DIR} -o runcons cons.c -lrt

.PHONY: clean
clean:
	rm runprod runcons
