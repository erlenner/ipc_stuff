#include <stdio.h>
#include <signal.h>

#include "ring_queue.h"
#include "ipc.h"
#include "debug.h"

ring_queue_def(int, 64) ring_queue;

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  ring_queue* queue = (ring_queue*)ipc_create(sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);
  ring_queue_init(queue);

  signal(SIGINT, sig_handler);

  while (run)
  {
    static int entry=0;
    int err;
    ring_queue_push(queue, entry++, err);
    debug_assert(err == 0);
    usleep(3 * 1000);
  }

  ipc_destroy((void*)queue, sizeof(ring_queue));

  return 0;
}
