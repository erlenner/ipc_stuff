#include "ring_queue.h"
#include "ipc.h"


#define RING_SIZE 64
ring_queue_def(int, RING_SIZE) ring_queue;

int main()
{
  
  ring_queue* queue = (ring_queue*)ipc_open(sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);


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

  sleep(2);

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      int err;
      ring_queue_eat(queue, entry, err);
      printf("err, eaten: %d %d\n", err, entry);
    }
  }

  ipc_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
