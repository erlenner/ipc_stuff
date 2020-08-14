#include <stdio.h>

#include "ring_queue.h"



//#define RING_STORAGE char
//#define RING_SIZE 128
//#include "ring_queue_impl.h"
//#undef RING_SIZE
//#undef RING_STORAGE


int main()
{
  
  printf("main\n");


  GET_RING_QUEUE(int, 64) queue;
  RING_QUEUE(init, &queue);

  //{
  //  int entry;
  //  entry=5;
  //  GET_RING_QUEUE_METHOD(push, int, 64)(&queue, &entry);
  //  entry=3;
  //  GET_RING_QUEUE_METHOD(push,int,64)(&queue, &entry);
  //  entry=127;
  //  GET_RING_QUEUE_METHOD(push,int,64)(&queue, &entry);
  //  entry=128;
  //  GET_RING_QUEUE_METHOD(push,int,64)(&queue, &entry);
  //}

  //{
  //  int entry;
  //  for (int i=0; i<7; ++i)
  //  {
  //    int ret = GET_RING_QUEUE_METHOD(eat,int,64)(&queue, &entry);
  //    printf("ret, eaten: %d %d\n", ret, entry);
  //  }
  //}

  //printf("\n");
  //{
  //  int entry;
  //  entry=45;
  //  GET_RING_QUEUE_METHOD(push,int,64)(&queue, &entry);
  //  entry=35;
  //  GET_RING_QUEUE_METHOD(push,int,64)(&queue, &entry);
  //  entry=25;
  //  GET_RING_QUEUE_METHOD(push,int,64)(&queue, &entry);
  //}

  //{
  //  int entry;
  //  for (int i=0; i<7; ++i)
  //  {
  //    int ret = GET_RING_QUEUE_METHOD(eat,int,64)(&queue, &entry);
  //    printf("ret, eaten: %d %d\n", ret, entry);
  //  }
  //}
  //for (int i=0; i<64; ++i)
  //  printf("%d ", queue.buffer[i]);

  //printf("\n");

  printf("done\n");
  return 0;
}
