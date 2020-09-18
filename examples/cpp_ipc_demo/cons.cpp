#include <stdio.h>
#include <signal.h>

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
  //ipc_receiver<my_struct> topic("/my_topic");
  ipc_receiver<my_struct> topic;
  debug_assert(topic.init("/my_topic") == 0, return -1);

  signal(SIGINT, sig_handler);


  while (run)
  {
    my_struct entry;

    int seq = topic.read(entry);

    static int last_seq = 0;
    static my_struct last_entry;
    memset(&last_entry, 0, sizeof(last_entry));

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
      //usleep(3 * 1000);
    }

    //usleep(3 * 1000);
  }

  return 0;
}
