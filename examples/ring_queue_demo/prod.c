#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#include "ring_queue.h"
#include "ipc.h"
#include "debug.h"

#include "my_struct.h"

ring_queue_def(my_struct, 64) ring_queue;

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  ring_queue* queue = (ring_queue*)ipc_create("/ipc_test", sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);
  //ring_queue_init(queue); // unnecessary with shared memory since ftruncate already gives zero-ed bytes

  signal(SIGINT, sig_handler);

  my_struct entry;
  memset(&entry, 0, sizeof(entry));

  srand(time(NULL));

  while (run)
  {

    int err;
    ring_queue_push(queue, entry, err);
    debug_assert_v((err == 0), "entry: %d\tri: %d\twi: %d ", entry.data[0].ii, queue->read_index, queue->write_index);
    if (err == 0)
    {
      for (int i=0; i<50; ++i)
      {
        ++(entry.data[i].ii);
        ++(entry.data[i].iii);
        ++(entry.data[i].iiii);
      }
    }
    //else
    //  usleep(3 * 1000);
    if ((rand() % 1000) == 0) usleep((rand() % 10) * 1000);
  }

  debug("read_index: %d write_index: %d\n", queue->read_index, queue->write_index);

  ipc_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
