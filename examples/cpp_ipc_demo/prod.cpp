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
  //ipc_writer<my_struct> writer("my_topic");
  ipc_writer<my_struct> writer;
  debug_assert(writer.init("my_topic") == 0, return -1);

  my_struct entry;
  memset(&entry, 0, sizeof(entry));

  signal(SIGINT, sig_handler);

  while (run)
  {
    writer.write(entry);

    for (int i=0; i<50; ++i)
    {
      ++(entry.data[i].ii);
      ++(entry.data[i].iii);
      ++(entry.data[i].iiii);
    }
    debug("sending entry:\t");
    my_struct_print(printf, entry, "\n");

    usleep(1 * 1000);
  }

  return 0;
}
