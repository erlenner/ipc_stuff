#include <stdio.h>
#include <signal.h>

#include "seq_lock.h"
#include "shmem.h"
#include "debug.h"

#include "my_struct.h"

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  seq_lock_def(my_struct) seq_lock;

  seq_lock* lock = (seq_lock*)shmem_create("/shmem_test", sizeof(seq_lock));
  debug_assert(lock != NULL, return -1);
  //seq_lock_init(lock); // unnecessary with shared memory since ftruncate already gives zero-ed bytes

  my_struct entry;
  memset(&entry, 0, sizeof(entry));

  signal(SIGINT, sig_handler);

  while (run)
  {
    seq_lock_write(lock, entry);
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

  shmem_unmap((void*)lock, sizeof(seq_lock));

  return 0;
}
