#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#include "ipc/ring_queue.h"
#include "ipc/shmem.h"
#include "ipc/debug.h"

#include "my_struct.h"

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  ring_queue_def(my_struct, 64) ring_queue;
  ring_queue* queue = (ring_queue*)shmem_open("/shmem_test", sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);

  debug("read_index: %d write_index: %d\n", queue->read_index, queue->write_index);

  ring_queue_cache_def(queue) ring_queue_cache;
  ring_queue_cache cache;
  memset(cache, 0, sizeof(cache));

  my_struct last_entry;
  memset(&last_entry, 0, sizeof(last_entry));

  srand(time(NULL));

  signal(SIGINT, sig_handler);

  while (run)
  {
    int nread;
    do
    {
      ring_queue_eat_all(queue, cache, nread);

      for (int n=0; n < nread; ++n)
      {
        //printf("%d\t", n);
        //my_struct_print(printf, cache[n], "\n");

        for (int i=0; i < 50; ++i)
        {
          #define assert_monotonic(value, last_value) debug_assert_v(value == last_value + 1, "non-monotonic: %d != %d + 1. ", value, last_value)
          assert_monotonic(cache[n].data[i].ii, last_entry.data[i].ii);
          assert_monotonic(cache[n].data[i].iii, last_entry.data[i].iii);
          assert_monotonic(cache[n].data[i].iiii, last_entry.data[i].iiii);
          #undef assert_monotonic
        }
        last_entry = cache[n];
      }
    }
    while (nread > 0 && run);
    //while (0);
    //printf("\n");
    //usleep(30 * 1000);
    //if ((rand() % 1000) == 0) usleep((rand() % 10) * 1000);
  }

  debug("read_index: %d write_index: %d entry: ", queue->read_index, queue->write_index);
  my_struct_print(printf, last_entry, "\n");

  shmem_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
