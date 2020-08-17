#include <stdio.h>

#include "ring_queue.h"
#include "ipc.h"

#define RING_SIZE 8
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
    int err;
    int entry;

    entry=5;
    ring_queue_push(queue, entry, err);
    debug_assert(err == 0);
    entry=3;
    ring_queue_push(queue, entry, err);
    debug_assert(err == 0);
    entry=127;
    ring_queue_push(queue, entry, err);
    debug_assert(err == 0);
    entry=110;
    ring_queue_push(queue, entry, err);
    debug_assert(err == 0);
  }

  for (int i=0; i<queue->size; ++i)
    printf("%d ", queue->buffer[i]);
  printf("\n");

  sleep(2);

  {
    int err;
    int entry;
    entry=45;
    ring_queue_push(queue, entry, err);
    debug_assert(err == 0);
    entry=35;
    ring_queue_push(queue, entry, err);
    debug_assert(err == 0);
    entry=25;
    ring_queue_push(queue, entry, err);
    debug_assert(err == 0);
  }

  sleep(2);

  for (int i=0; i<queue->size; ++i)
    printf("%d ", queue->buffer[i]);
  printf("\n");






  ipc_destroy((void*)queue, sizeof(ring_queue));

  return 0;
}
