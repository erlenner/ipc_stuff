/*
Ring Queue Cache

Extends ring queue with a cache buffer that the consumer can use to read all
queued elements at once so it can release them before using them.

*/

#pragma once
#include "ipc/ring_queue.h"

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
