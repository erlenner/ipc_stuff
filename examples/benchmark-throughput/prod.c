#include <stdio.h>
#include <signal.h>

#include "ring_queue.h"
#include "shmem.h"
#include "debug.h"
#include "ms.h"

ring_queue_def(int, 64) ring_queue;

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  ring_queue* queue = (ring_queue*)shmem_create("/shmem_test", sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);
  //ring_queue_init(queue); // unnecessary with shared memory since ftruncate already gives zero-ed bytes

  signal(SIGINT, sig_handler);

  double t;
  int entry=0;
  while (run && (entry < (63 + (int)1e8)))
  {
    int err;
    ring_queue_push(queue, entry, err);
    if (!err)
      ++entry;
    //debug_assert_v((err == 0) && ++entry, "entry: %d ", entry);
    //usleep(3 * 1000);
    if (entry == 64)
      t = ms();
  }
  ms_toc(&t);

  debug("read_index: %d write_index: %d\n", queue->read_index, queue->write_index);

  shmem_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
