#include <stdio.h>

#include "ring_queue.h"
#include "ipc.h"

#define RING_SIZE 64
ring_queue_def(int, RING_SIZE) ring_queue;


int main()
{
  ring_queue* queue = (ring_queue*)ipc_create(sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);

  {
    // initialize struct
    ring_queue tmp = ring_queue_inst(int, RING_SIZE);
    memcpy(queue, &tmp, sizeof(ring_queue));
  }

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

  sleep(2);

  {
    int entry;
    entry=45;
    ring_queue_push(queue, entry);
    entry=35;
    ring_queue_push(queue, entry);
    entry=25;
    ring_queue_push(queue, entry);
  }

  sleep(2);

  for (int i=0; i<queue->size; ++i)
    printf("%d ", queue->buffer[i]);
  printf("\n");






  ipc_destroy((void*)queue, sizeof(ring_queue));

  return 0;
}
