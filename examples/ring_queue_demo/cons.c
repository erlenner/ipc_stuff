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
  ring_queue* queue = (ring_queue*)ipc_open("/ipc_test", sizeof(ring_queue));
  debug_assert(queue != NULL, return -1);

  debug("read_index: %d write_index: %d\n", queue->read_index, queue->write_index);

  signal(SIGINT, sig_handler);

  my_struct entry, last_entry;
  memset(&entry, 0, sizeof(entry));
  memset(&last_entry, 0, sizeof(last_entry));

  srand(time(NULL));

  while (run)
  {
    int err;
    do
    {
      ring_queue_eat(queue, entry, err);
      //my_struct_print(printf, entry, "\n");

      if (err == 0)
      {
        for (int i=0; i<50; ++i)
        {
          #define assert_monotonic(value, last_value) debug_assert_v(value == last_value + 1, "non-monotonic: %d != %d + 1. ", value, last_value)
          assert_monotonic(entry.data[i].ii, last_entry.data[i].ii);
          assert_monotonic(entry.data[i].l, last_entry.data[i].l);
          assert_monotonic(entry.data[i].ll, last_entry.data[i].ll);
          #undef assert_monotonic
        }
      }
      last_entry = entry;
    }
    while (err == 0 && run);
    //printf("\n");
    //usleep(30 * 1000);
    if ((rand() % 1000) == 0) usleep((rand() % 10) * 1000);
  }

  debug("read_index: %d write_index: %d entry: %d\n", queue->read_index, queue->write_index, entry);

  ipc_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
