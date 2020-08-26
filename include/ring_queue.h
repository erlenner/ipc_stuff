/*
Ring Queue

Inspired by:
https://github.com/boostorg/lockfree/blob/develop/include/boost/lockfree/spsc_queue.hpp
*/

#pragma once
#include <assert.h> // for static_assert
#include <stdatomic.h>

#define MX6_CACHELINE_BYTES 32
#define CACHELINE_BYTES MX6_CACHELINE_BYTES
// L1: https://developer.arm.com/documentation/ddi0388/f/Level-1-Memory-System/About-the-L1-memory-system
// L2: https://community.nxp.com/thread/510105

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
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  int wi = atomic_load_explicit(&(q)->write_index, memory_order_relaxed);           \
  wi = next_index(wi, ring_queue_size(q));                                          \
                                                                                    \
  if (wi == atomic_load_explicit(&(q)->read_index, memory_order_acquire))           \
    OPT_SET(1, __VA_ARGS__)                                                         \
  else                                                                              \
  {                                                                                 \
    (q)->buffer[wi] = e;                                                            \
    atomic_store_explicit(&(q)->write_index, wi, memory_order_release);             \
    OPT_SET(0, __VA_ARGS__)                                                         \
  }                                                                                 \
                                                                                    \
} while(0)

#define ring_queue_eat(_queue, _entry, .../*error*/)                                \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  const int wi = atomic_load_explicit(&(q)->write_index, memory_order_acquire);     \
  int ri = atomic_load_explicit(&(q)->read_index, memory_order_relaxed);            \
                                                                                    \
  if (ri == wi)                                                                     \
    OPT_SET(1, __VA_ARGS__)                                                         \
  else                                                                              \
  {                                                                                 \
    ri = next_index(ri, ring_queue_size(q));                                        \
    e = (q)->buffer[ri];                                                            \
    atomic_store_explicit(&(q)->read_index, ri, memory_order_release);              \
    OPT_SET(0, __VA_ARGS__)                                                         \
  }                                                                                 \
                                                                                    \
  _entry = e;                                                                       \
} while(0)

#define ring_queue_eat_last(_queue, _entry, .../*error*/)                           \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  const int wi = atomic_load_explicit(&q->write_index, memory_order_acquire);       \
  int ri = atomic_load_explicit(&q->read_index, memory_order_relaxed);              \
                                                                                    \
  if (ri == wi)                                                                     \
    OPT_SET(1, __VA_ARGS__)                                                         \
  else                                                                              \
  {                                                                                 \
    ri = wi;                                                                        \
    e = q->buffer[ri];                                                              \
    atomic_store_explicit(&q->read_index, ri, memory_order_release);                \
    OPT_SET(0, __VA_ARGS__)                                                         \
  }                                                                                 \
                                                                                    \
  _entry = e;                                                                       \
} while(0)




#define ring_queue_cache_def(queue) typedef typeof(queue->buffer)

#define ring_queue_eat_all(_queue, _cache, _nread)                                  \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(&(q->buffer[0])) c = _cache;                                               \
                                                                                    \
  const int wi = atomic_load_explicit(&q->write_index, memory_order_acquire);       \
  int ri = atomic_load_explicit(&q->read_index, memory_order_relaxed);              \
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
    atomic_store_explicit(&q->read_index, ri, memory_order_release);                \
  }                                                                                 \
} while(0)
