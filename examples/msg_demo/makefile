
IPC_DIR=../../include

.PHONY: all
all: runprod runcons

runprod: prod.cpp ${IPC_DIR}/msg.h ${IPC_DIR}/macro.h ${IPC_DIR}/debug.h
	gcc -Wall -O2 -Wall -I${IPC_DIR} -o runprod prod.cpp -lrt

.PHONY: clean
clean:
	rm runprod runcons
