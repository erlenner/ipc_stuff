#include <stdio.h>
#include <signal.h>

#include "ipc/big_seq_lock.h"
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
  typedef big_seq_lock<my_struct, 64> sl;

  sl* lock = (sl*)shmem_create("/shmem_test", sizeof(sl));
  debug_assert(lock != NULL, return -1);

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

  shmem_unmap((void*)lock, sizeof(sl));

  return 0;
}
