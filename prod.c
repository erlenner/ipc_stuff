#include <stdio.h>

#define RING_STORAGE int
#define RING_SIZE 64
#include "ring_queue.h"
RING_QUEUE queue;
#undef RING_QUEUE
#undef RING_STORAGE
#undef RING_SIZE

int main()
{
  
  printf("main\n");


  init(&queue);

  {
    int entry;
    entry=5;
    push(&queue, &entry);
    entry=3;
    push(&queue, &entry);
    entry=127;
    push(&queue, &entry);
    entry=128;
    push(&queue, &entry);
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int ret = eat(&queue, &entry);
      printf("ret, eaten: %d %d\n", ret, entry);
    }
  }

  printf("\n");
  {
    int entry;
    entry=45;
    push(&queue, &entry);
    entry=35;
    push(&queue, &entry);
    entry=25;
    push(&queue, &entry);
  }

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int ret = eat(&queue, &entry);
      printf("ret, eaten: %d %d\n", ret, entry);
    }
  }
  for (int i=0; i<64; ++i)
    printf("%d ", queue.buffer[i]);

  printf("\n");

  printf("done\n");
  return 0;
}
