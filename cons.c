#include "ring_queue.h"
#include "ipc.h"


#define RING_SIZE 4
ring_queue_def(int, RING_SIZE) ring_queue;

int main()
{
  
  ring_queue* queue = (ring_queue*)ipc_open(sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);


  {
    printf("cons: ");
    int entry;
    int err;
    do
    {
      ring_queue_eat(queue, entry, err);
      printf(" %d", entry);
    }
    while (err == 0);
    printf("\n");
  }

  sleep(2);

  {
    printf("cons: ");
    int entry;
    int err;
    do
    {
      ring_queue_eat(queue, entry, err);
      printf(" %d", entry);
    }
    while (err == 0);
    printf("\n");
  }

  ipc_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
