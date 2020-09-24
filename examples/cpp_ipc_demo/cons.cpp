#include <signal.h>

#define debug_stdout stdout
#define debug_stderr stderr
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

    if (reader.read(entry) == 0)
    //while (reader.read(entry) == 0) // if using ipc_reader_rq
    {
      debug("new entry:\t");
      my_struct_print(debug_plain, entry, "\n");
    }

    usleep(3 * 1000);
  }

  return 0;
}
