//#include <stdio.h>

#define RING_STORAGE int
#define RING_SIZE 64
#include "ring_queue.h"

//#define RING_STORAGE char
//#define RING_SIZE 128
//#include "ring_queue_impl.h"
//#undef RING_SIZE
//#undef RING_STORAGE


int main()
{
  
  printf("main\n");


  RING_QUEUE queue;
  RING_QUEUE_METHOD(init)(&queue);
  //ring_queue_char_128_static_debug();

  {
    int entry;
    entry=5;
    RING_QUEUE_METHOD(push)(&queue, &entry);
    entry=3;
    RING_QUEUE_METHOD(push)(&queue, &entry);
    entry=127;
    RING_QUEUE_METHOD(push)(&queue, &entry);
    entry=128;
    RING_QUEUE_METHOD(push)(&queue, &entry);
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int ret = RING_QUEUE_METHOD(eat)(&queue, &entry);
      printf("ret, eaten: %d %d\n", ret, entry);
    }
  }

  printf("\n");
  {
    int entry;
    entry=45;
    RING_QUEUE_METHOD(push)(&queue, &entry);
    entry=35;
    RING_QUEUE_METHOD(push)(&queue, &entry);
    entry=25;
    RING_QUEUE_METHOD(push)(&queue, &entry);
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int ret = RING_QUEUE_METHOD(eat)(&queue, &entry);
      printf("ret, eaten: %d %d\n", ret, entry);
    }
  }
  for (int i=0; i<64; ++i)
    printf("%d ", queue.buffer[i]);

  printf("\n");

  printf("done\n");
  return 0;
}
