/*
Ring Queue

Inspired by:
https://github.com/boostorg/lockfree/blob/develop/include/boost/lockfree/spsc_queue.hpp
*/

#pragma once
#include <assert.h> // for static_assert
#include "smp.h"

#define ring_queue_def(STORAGE, SIZE)\
static_assert((SIZE & (SIZE - 1)) == 0, "SIZE not binary exponent (2^n)");  \
typedef struct                                                              \
{                                                                           \
  int read_index;                                                           \
  char padding[CACHELINE_BYTES - sizeof(int)];                              \
  int write_index;                                                          \
  STORAGE buffer[SIZE];                                                     \
} __attribute__ ((aligned(CACHELINE_BYTES)))


#define ring_queue_size(queue) (sizeof((queue)->buffer) / sizeof((queue)->buffer[0]))

#define ring_queue_init(queue)  \
do {                            \
  (queue)->read_index = 0;      \
  (queue)->write_index = 0;     \
} while (0)


// OPT_set(var) : var = 1;
// OPT_set() : ;
#define OPT_SET(set, ...) { __VA_ARGS__ __VA_OPT__(= set) ; }


#define next_index(index, size) (((index) + 1) & ((size)-1))

#define ring_queue_push(_queue, _entry, .../*error*/)                               \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
                                                                                    \
  int wi = (q)->write_index;                                                        \
  const int ri = smp_load_acquire(q->read_index);                                   \
                                                                                    \
  wi = next_index(wi, ring_queue_size(q));                                          \
                                                                                    \
  if (wi == ri)                                                                     \
    OPT_SET(1, __VA_ARGS__)                                                         \
  else                                                                              \
  {                                                                                 \
    (q)->buffer[wi] = _entry;                                                       \
    smp_store_release(q->write_index, wi);                                          \
    OPT_SET(0, __VA_ARGS__)                                                         \
  }                                                                                 \
                                                                                    \
} while(0)

#define ring_queue_eat(_queue, _entry, .../*error*/)                                \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
                                                                                    \
  int ri = (q)->read_index;                                                         \
  const int wi = smp_load_acquire(q->write_index);                                  \
                                                                                    \
  if (ri == wi)                                                                     \
    OPT_SET(1, __VA_ARGS__)                                                         \
  else                                                                              \
  {                                                                                 \
    ri = next_index(ri, ring_queue_size(q));                                        \
    _entry = (q)->buffer[ri];                                                       \
    smp_store_release(q->read_index, ri);                                           \
    OPT_SET(0, __VA_ARGS__)                                                         \
  }                                                                                 \
} while(0)

#define ring_queue_eat_last(_queue, _entry, .../*error*/)                           \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
                                                                                    \
  const int wi = smp_load_acquire(q->write_index);                                  \
  int ri = q->read_index;                                                           \
                                                                                    \
  if (ri == wi)                                                                     \
    OPT_SET(1, __VA_ARGS__)                                                         \
  else                                                                              \
  {                                                                                 \
    ri = wi;                                                                        \
    _entry = q->buffer[ri];                                                         \
    smp_store_release(q->read_index, ri);                                           \
    OPT_SET(0, __VA_ARGS__)                                                         \
  }                                                                                 \
} while(0)




#define ring_queue_cache_def(queue) typedef typeof(queue->buffer)

#define ring_queue_eat_all(_queue, _cache, _nread)                                  \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(&(q->buffer[0])) c = _cache;                                               \
                                                                                    \
  const int wi = smp_load_acquire(q->write_index);                                  \
  int ri = q->read_index;                                                           \
                                                                                    \
  if (ri == wi)                                                                     \
    _nread = 0;                                                                     \
  else                                                                              \
  {                                                                                 \
    const int ri_count = ri + 1;                                                    \
    if (ri < wi)                                                                    \
    {                                                                               \
      _nread = wi-ri;                                                               \
      memcpy(c, q->buffer + ri_count, _nread * sizeof(c[0]));                       \
    }                                                                               \
    else                                                                            \
    {                                                                               \
      const int wi_end_count = ring_queue_size(q) - ri_count;                       \
      memcpy(c, q->buffer + ri_count, wi_end_count * sizeof(c[0]));                 \
                                                                                    \
      const int wi_count = wi + 1;                                                  \
      memcpy(c + wi_end_count, q->buffer, wi_count * sizeof(c[0]));                 \
                                                                                    \
      _nread = wi_end_count + wi_count;                                             \
    }                                                                               \
                                                                                    \
    ri = wi;                                                                        \
    smp_store_release(q->read_index, ri);                                           \
  }                                                                                 \
} while(0)
