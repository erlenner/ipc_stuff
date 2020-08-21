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

  int entry;

  while (run)
  {
    opt_queue_read(queue, entry);

    printf("%d\n", entry);

    //usleep(3 * 1000);
  }

  ipc_unmap((void*)queue, sizeof(opt_queue));

  return 0;
}
