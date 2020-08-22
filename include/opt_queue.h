/*
Optimistic Queue

Inspired by:
http://www.1024cores.net/home/lock-free-algorithms/reader-writer-problem/improved-lock-free-seqlock
*/

#pragma once
#include <assert.h> // for static_assert
#include <stdatomic.h>

#define MX6_CACHELINE_BYTES 32
#define CACHELINE_BYTES MX6_CACHELINE_BYTES
// L1: https://developer.arm.com/documentation/ddi0388/f/Level-1-Memory-System/About-the-L1-memory-system
// L2: https://community.nxp.com/thread/510105

#define opt_queue_def(STORAGE, SIZE)\
static_assert((SIZE & (SIZE - 1)) == 0, "SIZE not binary exponent (2^n)");  \
typedef struct                                                              \
{                                                                           \
  int write_index;                                                          \
                                                                            \
  struct                                                                    \
  {                                                                         \
    int seq;                                                                \
    STORAGE entry;                                                          \
  } buffer[SIZE];                                                           \
                                                                            \
} __attribute__ ((aligned(CACHELINE_BYTES)))


#define opt_queue_size(queue) (sizeof((queue)->buffer) / sizeof((queue)->buffer[0]))

#define opt_queue_init(queue)                                   \
do {                                                            \
  (queue)->write_index = 0;                                     \
  memset((queue)->integrity_counter, 0, opt_queue_size(queue)); \
} while (0)


#define next_index(index, size) (((index) + 1) & ((size)-1))

#define opt_queue_write(_queue, _entry)                                             \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  int wi = atomic_load_explicit(&(q)->write_index, __ATOMIC_RELAXED);               \
  wi = next_index(wi, opt_queue_size(q));                                           \
                                                                                    \
  static int seq = 0;                                                               \
  atomic_store_explicit(&(q)->buffer[wi].seq, ++seq, memory_order_release);         \
  (q)->buffer[wi].entry = e;                                                        \
  atomic_store_explicit(&(q)->buffer[wi].seq, ++seq, memory_order_release);         \
  atomic_store_explicit(&(q)->write_index, wi, memory_order_release);               \
                                                                                    \
} while(0)

#define opt_queue_read(_queue, _entry)                                              \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  while(1)                                                                          \
  {                                                                                 \
    /*static int total = 0;*/                                                       \
    /*++total;*/                                                                    \
    const int wi = atomic_load_explicit(&(q)->write_index, memory_order_acquire);   \
    int seq = atomic_load_explicit(&(q)->buffer[wi].seq, memory_order_acquire);     \
    if (seq & 1)                                                                    \
    {                                                                               \
      fprintf(stderr, "V");                                                         \
      continue;                                                                     \
    }                                                                               \
                                                                                    \
    e = (q)->buffer[wi].entry;                                                      \
                                                                                    \
    int seq2 = atomic_load_explicit(&(q)->buffer[wi].seq, memory_order_acquire);    \
    if (seq2 == seq)                                                                \
      break;                                                                        \
    /*static int fail = 0;*/                                                        \
    /*fprintf(stderr, "\nFAILED: %d %d %f\n", ++fail, total, (total > 0) ? ((float)fail / (float)total) : 0);*/ \
  }                                                                                 \
                                                                                    \
  _entry = e;                                                                       \
  /*fprintf(stderr, "i");*/                                                         \
  /*printf("ok %d\n", _entry);*/                                                    \
} while(0)
