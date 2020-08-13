#include <stdio.h>

#include "macro_meta_programming.h"

_Static_assert ((RING_SIZE & (RING_SIZE - 1)) == 0, "RING_SIZE is a binary exponent (2^n)");

#define RING_QUEUE CAT6(ring_queue, _, RING_STORAGE, _, RING_SIZE, _)
#define RING_QUEUE_STORAGE_SIZE_(function) CAT2(RING_QUEUE, function)

#if !defined(PREV_RING_QUEUE) || (RING_QUEUE != PREV_RING_QUEUE)

typedef struct
{
  int read_index;
  int write_index;
  RING_STORAGE buffer[RING_SIZE];
} RING_QUEUE;

int RING_QUEUE_STORAGE_SIZE_(static_debug)()
{
  printf("size of: %d %d %d\n", sizeof(RING_STORAGE), sizeof(RING_STORAGE*), sizeof(RING_QUEUE));
}


// interface inspired by boost: https://github.com/boostorg/lockfree/blob/develop/include/boost/lockfree/spsc_queue.hpp

//#define next_index(index, size) (((index) + 1) & ((size)-1))
//
//inline int ring_push(RING_QUEUE *ring_queue, const RING_STORAGE *entry)\
//{
//  int wi = __atomic_load_n(&ring_queue->write_index, __ATOMIC_RELAXED);
//  int next = next_index(ring_queue->write_index, RING_SIZE);
//
//  if (next == __atomic_load_n(&ring_queue->read_index, __ATOMIC_ACQUIRE))
//    return 1;
//
//  ring_queue->buffer[wi] = *entry;
//  __atomic_store_n(&ring_queue->write_index, next, __ATOMIC_RELEASE);
//}
//
//inline int ring_eat(RING_QUEUE *ring_queue, RING_STORAGE *entry)
//{
//  int wi = __atomic_load_n(&ring_queue->write_index, __ATOMIC_ACQUIRE);
//  int ri = __atomic_load_n(&ring_queue->read_index, __ATOMIC_RELAXED);
//
//  if (ri == wi)
//    return 1;
//
//  *entry = ring_queue->buffer[ri];
//  int next = next_index(ring_queue->read_index, RING_SIZE);
//  __atomic_store_n(&ring_queue->read_index, next, __ATOMIC_RELEASE);
//}

#undef RING_QUEUE
#undef RING_QUEUE_STORAGE_SIZE_

#endif
