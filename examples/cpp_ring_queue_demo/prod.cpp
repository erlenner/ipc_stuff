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
  typedef ring_queue<my_struct, 64> rq;

  rq* queue = (rq*)shmem_create("/shmem_test", sizeof(rq));
  debug_assert(queue != NULL, return -1);

  signal(SIGINT, sig_handler);

  my_struct entry;
  memset(&entry, 0, sizeof(entry));

  srand(time(NULL));

  while (run)
  {

    int err;
    err = queue->write(entry);
    debug_assert_v((err == 0), "entry: %d ", entry.data[0].ii);
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
    //if ((rand() % 1000) == 0) usleep((rand() % 10) * 1000);
    usleep(3 * 1000);
  }

  shmem_unmap((void*)queue, sizeof(rq));

  return 0;
}
