#include <stdio.h>

#include "ring_queue.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"


//#define RING_STORAGE char
//#define RING_SIZE 128
//#include "ring_queue_impl.h"
//#undef RING_SIZE
//#undef RING_STORAGE


int main()
{
  
  printf("main\n");


  GET_RING_QUEUE(int, 64) queue;
  RING_QUEUE(init, &queue);

  //GET_RING_QUEUE(char, 128) queue2;
  //RING_QUEUE(init, &queue2);

  {
    int entry;
    entry=5;
    RING_QUEUE(push, &queue, &entry);
    entry=3;
    RING_QUEUE(push, &queue, &entry);
    entry=127;
    RING_QUEUE(push, &queue, &entry);
    entry=128;
    RING_QUEUE(push, &queue, &entry);
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int ret = RING_QUEUE(eat, &queue, &entry);
      printf("ret, eaten: %d %d\n", ret, entry);
    }
  }

  printf("\n");
  {
    int entry;
    entry=45;
    RING_QUEUE(push, &queue, &entry);
    entry=35;
    RING_QUEUE(push, &queue, &entry);
    entry=25;
    RING_QUEUE(push, &queue, &entry);
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int ret = RING_QUEUE(eat, &queue, &entry);
      printf("ret, eaten: %d %d\n", ret, entry);
    }
  }
  for (int i=0; i<64; ++i)
    printf("%d ", queue.buffer[i]);

  printf("\n");

  printf("done\n");
  return 0;
}

#pragma GCC diagnostic pop
