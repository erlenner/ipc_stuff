
IPC_DIR=../../include

.PHONY: all
all: runprod runcons

runprod: prod.cpp ${IPC_DIR}/big_seq_lock.h ${IPC_DIR}/shmem.h ${IPC_DIR}/debug.h
	g++ -Wall -O2 -Wall -I${IPC_DIR} -o runprod prod.cpp -lrt

runcons: cons.cpp ${IPC_DIR}/big_seq_lock.h ${IPC_DIR}/shmem.h ${IPC_DIR}/debug.h
	g++ -Wall -O2 -Wall -I${IPC_DIR} -o runcons cons.cpp -lrt

.PHONY: clean
clean:
	rm runprod runcons
