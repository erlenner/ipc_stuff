#include <stdio.h>
#include <signal.h>

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

  while (run)
  {
    int err;
    do
    {
      ring_queue_eat(queue, entry, err);
      my_struct_print(printf, entry, "\n");

      if (err == 0)
      {
        for (int i=0; i<50; ++i)
        {
          #define datai entry.data[i]
          #define last_datai last_entry.data[i]
          debug_assert_v(datai.ii == last_datai.ii + 1, "non-monotonic: %d != %d + 1. ", datai.ii, last_datai.ii);
          debug_assert_v(datai.l == last_datai.l + 1, "non-monotonic: %d != %d + 1. ", datai.l, last_datai.l);
          debug_assert_v(datai.ll == last_datai.ll + 1, "non-monotonic: %d != %d + 1. ", datai.ll, last_datai.ll);
          #undef last_datai
          #undef datai
        }
      }
      last_entry = entry;
    }
    while (err == 0 && run);
    printf("\n");
    usleep(30 * 1000);
  }

  debug("read_index: %d write_index: %d entry: %d\n", queue->read_index, queue->write_index, entry);

  ipc_unmap((void*)queue, sizeof(ring_queue));

  return 0;
}
