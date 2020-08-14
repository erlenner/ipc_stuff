#include <stdio.h>

#include "ring_queue.h"


//#define RING_STORAGE char
//#define RING_SIZE 128
//#include "ring_queue_impl.h"
//#undef RING_SIZE
//#undef RING_STORAGE

ring_queue_def(int, 64) ring_queue;


int main()
{
  
  printf("main\n");


  ring_queue queue = ring_queue_inst(int, 64);

  {
    int entry;
    entry=5;
    int err;
    ring_queue_push(queue, entry, err);
    printf("err: %d\n", err);

    entry=3;
    ring_queue_push(queue, entry);
    entry=127;
    ring_queue_push(queue, entry);
    entry=128;
    ring_queue_push(queue, entry);
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int err;
      ring_queue_eat(queue, entry, err);
      printf("err, eaten: %d %d\n", err, entry);
    }
  }

  printf("\n");
  {
    int entry;
    entry=45;
    ring_queue_push(queue, entry);
    entry=35;
    ring_queue_push(queue, entry);
    entry=25;
    ring_queue_push(queue, entry);
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int err;
      ring_queue_eat(queue, entry, err);
      printf("err, eaten: %d %d\n", err, entry);
    }
  }
  for (int i=0; i<64; ++i)
    printf("%d ", queue.buffer[i]);

  printf("\n");

  printf("done\n");
  return 0;
}
