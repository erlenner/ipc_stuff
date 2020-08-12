#include <stdio.h>

#include "ring_queue.h"


int main()
{
  
  printf("main\n");

  ring_define(char, 64);
  ring_init();

  printf("cnt: %d\n", read_available(25,51, ring_size));
  printf("cnt: %d\n", write_available(25,51, ring_size));

  ring_push(5);
  ring_push(3);
  ring_push(127);
  ring_push(128);

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      ring_eat(entry);
      printf("eaten: %d\n", entry);
    }
  }

  printf("\n");
  ring_push(45);
  ring_push(35);
  ring_push(25);

  {
    int entry;
    for (int i=0; i<7; ++i)
    {
      ring_eat(entry);
      printf("eaten: %d\n", entry);
    }
  }

  printf("summary:\n%d %d\n", read_index, write_index);

  for (int i=0; i<ring_size; ++i)
    printf("%d ", ring_buffer[i]);

  printf("done\n");
  return 0;
}
