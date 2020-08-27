#include <stdio.h>
#include <signal.h>

#include "opt_queue.h"
#include "ipc.h"
#include "debug.h"

opt_queue_def(int, 64) opt_queue;

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  opt_queue* queue = (opt_queue*)ipc_open("/ipc_test", sizeof(opt_queue));
  debug_assert(queue != NULL, return -1);

  signal(SIGINT, sig_handler);


  while (run)
  {
    int entry;
    static int last_entry = 0;

    static int last_seq = 0;
    int seq = 0;

    opt_queue_read(queue, entry, seq);

    if (seq != last_seq)
    {
      printf("%d\n", entry);
      debug_assert_v(entry != last_entry, "equal: %d == %d. ", entry, last_entry);
      last_entry = entry;
      last_seq = seq;
    }

    //usleep(3 * 1000);
  }

  ipc_unmap((void*)queue, sizeof(opt_queue));

  return 0;
}
