#include <stdio.h>
#include <signal.h>

#include "ring_queue.h"
#include "ipc.h"
#include "debug.h"

#define RING_SIZE 64
ring_queue_def(int, RING_SIZE) ring_queue;

ring_queue* queue;
int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  
  queue = (ring_queue*)ipc_open(sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);

  signal(SIGINT, sig_handler);

  while (run)
  {
    int entry;
    int err;
    do
    {
      ring_queue_eat(queue, entry, err);
      printf("%d ", entry);

      static int last_entry = -1;
      debug_assert((entry == last_entry + 1) || (err) || (last_entry == -1), return -1);
      last_entry = entry;
    }
    while (err == 0 && run);
    printf("\n");
    usleep(30 * 1000);
  }

  ipc_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}