run: prod.c ring_queue.h
	gcc -Wall -o runprod prod.c

#run: cons.c ring_queue.h
#	gcc -o runcons cons.c