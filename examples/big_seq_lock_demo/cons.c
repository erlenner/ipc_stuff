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
  big_seq_lock_def(my_struct, 64) big_seq_lock;
  big_seq_lock* lock = (big_seq_lock*)shmem_open("/shmem_test", sizeof(big_seq_lock));
  debug_assert(lock != NULL, return -1);

  signal(SIGINT, sig_handler);


  while (run)
  {
    my_struct entry;
    static my_struct last_entry;
    memset(&last_entry, 0, sizeof(last_entry));

    static int last_seq = 0;
    int seq = 0;

    big_seq_lock_read(lock, entry, seq);

    if (seq != last_seq)
    {
      my_struct_print(printf, entry, "\n");

      for (int i=0; i < 50; ++i)
      {
        #define assert_nonequal(value, last_value) debug_assert_v(value != last_value, "equal: %d == %d. ", value, last_value)
        assert_nonequal(entry.data[i].ii, last_entry.data[i].ii);
        assert_nonequal(entry.data[i].iii, last_entry.data[i].iii);
        assert_nonequal(entry.data[i].iiii, last_entry.data[i].iiii);
        #undef assert_nonequal

        #define assert_equal(value, last_value) debug_assert_v(value == last_value, "not equal: %d != %d. ", value, last_value)
        assert_equal(entry.data[i].ii, entry.data[i].iii);
        assert_equal(entry.data[i].ii, entry.data[i].iiii);
        #undef assert_equal
      }
      last_entry = entry;
      last_seq = seq;
    }

    //usleep(3 * 1000);
  }

  shmem_unmap((void*)lock, sizeof(big_seq_lock));

  return 0;
}
