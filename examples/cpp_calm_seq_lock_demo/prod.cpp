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
  typedef calm_seq_lock<my_struct, 64> sl;

  sl* lock = (sl*)ipc_create("/ipc_test", sizeof(sl));
  debug_assert(lock != NULL, return -1);
  //calm_seq_lock_init(lock); // unnecessary with shared memory since ftruncate already gives zero-ed bytes

  my_struct entry;
  memset(&entry, 0, sizeof(entry));

  signal(SIGINT, sig_handler);

  while (run)
  {
    lock->write(entry);
    for (int i=0; i<50; ++i)
    {
      ++(entry.data[i].ii);
      ++(entry.data[i].iii);
      ++(entry.data[i].iiii);
    }
    //debug("entry: %d\n", entry);
    //my_struct_print(printf, entry, "\n");
    //usleep(1 * 100);
  }

  ipc_unmap((void*)lock, sizeof(sl));

  return 0;
}
