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
  ring_queue* queue = (ring_queue*)ipc_open("/ipc_test", sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);

  debug("read_index: %d write_index: %d\n", queue->read_index, queue->write_index);

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
      debug_assert_v((entry == last_entry + 1) || (err) || (last_entry == -1), "non-monotonic: %d != %d + 1. ", entry, last_entry);
      last_entry = entry;
    }
    while (err == 0 && run);
    printf("\n");
    usleep(30 * 1000);
  }

  debug("read_index: %d write_index: %d\n", queue->read_index, queue->write_index);

  ipc_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
