#include "ring_queue.h"
#include "ipc.h"

int main()
{
  
  printf("main\n");

  const int size = 16;

  int *buf = ipc_open(size);
  debug_assert(buf != NULL, return -1);

  int data = buf[0];

  printf("Read from shared memory: %d\n", data);


  ipc_unmap(buf, size);

  return 0;
}
