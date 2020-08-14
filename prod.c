#include <stdio.h>

#include "ring_queue.h"


//#define RING_STORAGE char
//#define RING_SIZE 128
//#include "ring_queue_impl.h"
//#undef RING_SIZE
//#undef RING_STORAGE

RING_QUEUE_DEF(int, 64) ring_queue;


int main()
{
  
  printf("main\n");


  ring_queue queue = RING_QUEUE_INST(int, 64)

  {
    int entry;
    entry=5;
    int err;
    RING_QUEUE_PUSH(queue, entry, err)
    printf("err: %d\n", err);

    entry=3;
    RING_QUEUE_PUSH(queue, entry)
    entry=127;
    RING_QUEUE_PUSH(queue, entry)
    entry=128;
    RING_QUEUE_PUSH(queue, entry)
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int err;
      RING_QUEUE_EAT(queue, entry, err)
      printf("err, eaten: %d %d\n", err, entry);
    }
  }

  printf("\n");
  {
    int entry;
    entry=45;
    RING_QUEUE_PUSH(queue, entry)
    entry=35;
    RING_QUEUE_PUSH(queue, entry)
    entry=25;
    RING_QUEUE_PUSH(queue, entry)
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int err;
      RING_QUEUE_EAT(queue, entry, err)
      printf("err, eaten: %d %d\n", err, entry);
    }
  }
  for (int i=0; i<64; ++i)
    printf("%d ", queue.buffer[i]);

  printf("\n");

  printf("done\n");
  return 0;
}
