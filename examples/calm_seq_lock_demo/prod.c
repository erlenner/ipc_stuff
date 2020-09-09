#include <stdio.h>
#include <signal.h>

#include "calm_seq_lock.h"
#include "ipc.h"
#include "debug.h"

#include "my_struct.h"

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  calm_seq_lock_def(my_struct, 64) calm_seq_lock;

  calm_seq_lock* lock = (calm_seq_lock*)ipc_create("/ipc_test", sizeof(calm_seq_lock));
  debug_assert(lock != NULL, return -1);
  //calm_seq_lock_init(lock); // unnecessary with shared memory since ftruncate already gives zero-ed bytes

  my_struct entry;
  memset(&entry, 0, sizeof(entry));

  signal(SIGINT, sig_handler);

  while (run)
  {
    calm_seq_lock_write(lock, entry);
      for (int i=0; i<50; ++i)
      {
        ++(entry.data[i].ii);
        ++(entry.data[i].iii);
        ++(entry.data[i].iiii);
      }
    //debug("entry: %d\n", entry);
    //usleep(3 * 1000);
  }

  ipc_unmap((void*)lock, sizeof(calm_seq_lock));

  return 0;
}
