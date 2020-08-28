#include <stdio.h>
#include <signal.h>

#include "opt_queue.h"
#include "ipc.h"
#include "debug.h"
#include "msg.h"

int run = 1;

void sig_handler(int sig)
{
  run = 0;
}

int main()
{
  //printf("num args: %d\n", NUM_ARGS(4, 3, llljslksdfj, ldskjfallkdsfjlasdkfj, sadlkfjsjf));
  //printf("str args: %s\n", STRINGIFY_ALL(4, 3, llljslksdfj, ldskjfallkdsfjlasdkfj));


  {
    struct {int a; int b;} s = {.a=1, .b=2};
    printf("s: %d, %d\n", s.a, s.b);
  }


  msg_def(msg, "abc", int, a, float, b);
  msg.a = 5;
  msg.b = 3.14;
  printf("msg: %s, %d, %d, %f\n", msg.id, msg.size, msg.a, msg.b);

  //opt_queue_def(my_struct, 64) opt_queue;

  //opt_queue* queue = (opt_queue*)ipc_create("/ipc_test", sizeof(opt_queue));
  //debug_assert(queue != NULL, return -1);
  ////opt_queue_init(queue); // unnecessary with shared memory since ftruncate already gives zero-ed bytes

  //my_struct entry;
  //memset(&entry, 0, sizeof(entry));

  //signal(SIGINT, sig_handler);

  //while (run)
  //{
  //  opt_queue_write(queue, entry);
  //    for (int i=0; i<50; ++i)
  //    {
  //      ++(entry.data[i].ii);
  //      ++(entry.data[i].iii);
  //      ++(entry.data[i].iiii);
  //    }
  //  //debug("entry: %d\n", entry);
  //  //usleep(3 * 1000);
  //}

  //ipc_unmap((void*)queue, sizeof(opt_queue));

  return 0;
}
