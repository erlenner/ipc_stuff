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
  //ring_queue_init(queue);
  //debug_assert((queue->read_index == 0) && (queue->write_index == 0), return -1);

  signal(SIGINT, sig_handler);

  while (run)
  {
    static int entry=0;
    int err;
    ring_queue_push(queue, entry, err);
    debug_assert_v((err == 0) && ++entry, "entry: %d ", entry);
    usleep(3 * 1000);
  }

  debug("read_index: %d write_index: %d\n", queue->read_index, queue->write_index);

  ipc_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
