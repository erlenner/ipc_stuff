#include <stdio.h>
#include <signal.h>

#include "ipc/ipc.h"
#include "ipc/debug.h"

#include "my_struct.h"

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  //ipc_reader<my_struct> reader("my_topic");
  ipc_reader<my_struct> reader;
  debug_assert(reader.init("my_topic") == 0, return -1);

  signal(SIGINT, sig_handler);


  while (run)
  {
    my_struct entry;

    int seq = reader.read(entry);

    static int last_seq = 0;

    if (seq != last_seq)
    {
      last_seq = seq;

      printf("new entry:\t");
      my_struct_print(printf, entry, "\n");
    }

    usleep(3 * 1000);
  }

  return 0;
}
