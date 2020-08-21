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
  opt_queue* queue = (opt_queue*)ipc_create("/ipc_test", sizeof(opt_queue));
  debug_assert(queue != NULL, return -1);
  //opt_queue_init(queue); // unnecessary with shared memory since ftruncate already gives zero-ed bytes

  signal(SIGINT, sig_handler);

  while (run)
  {
    static int entry=0;
    opt_queue_write(queue, entry);
    ++entry;
    //debug("entry: %d\n", entry);
    //usleep(3 * 1000);
  }

  ipc_unmap((void*)queue, sizeof(opt_queue));

  return 0;
}
