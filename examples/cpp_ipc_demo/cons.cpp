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

    if (seq != last_seq)
    {
      printf("new entry:\t");
      my_struct_print(printf, entry, "\n");
    }

    usleep(3 * 1000);
  }

  return 0;
}
