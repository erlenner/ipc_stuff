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
  //ipc_sender<my_struct> topic("/my_topic");
  ipc_sender<my_struct> topic;
  debug_assert(topic.init("/my_topic") == 0, return -1);

  my_struct entry;
  memset(&entry, 0, sizeof(entry));

  signal(SIGINT, sig_handler);

  while (run)
  {
    topic.write(entry);
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

  return 0;
}
