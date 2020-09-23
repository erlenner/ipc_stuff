#include <stdio.h>
#include <signal.h>

#include "ipc/ring_queue.h"
#include "ipc/shmem.h"
#include "ipc/debug.h"

ring_queue_def(int, 64) ring_queue;

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  ring_queue* queue = (ring_queue*)shmem_open("/shmem_test", sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);

  debug("read_index: %d write_index: %d\n", queue->read_index, queue->write_index);

  signal(SIGINT, sig_handler);

  int entry;
  while (run)
  {
    int err;
    do
    {
      ring_queue_eat(queue, entry, err);
      //printf("%d ", entry);

      //static int last_entry = -1;
      //debug_assert_v((entry == last_entry + 1) || (err) || (last_entry == -1), "non-monotonic: %d != %d + 1. ", entry, last_entry);
      //last_entry = entry;
    }
    while (err == 0 && run);
    //printf("\n");
    //usleep(30 * 1000);
  }

  debug("read_index: %d write_index: %d entry: %d\n", queue->read_index, queue->write_index, entry);

  shmem_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
