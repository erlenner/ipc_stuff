#pragma once
#include <assert.h> // for static_assert

#define MX6_CACHELINE_BYTES 32
#define CACHELINE_BYTES MX6_CACHELINE_BYTES
// L1: https://developer.arm.com/documentation/ddi0388/f/Level-1-Memory-System/About-the-L1-memory-system
// L2: https://community.nxp.com/thread/510105

#define ring_queue_def(STORAGE, SIZE)\
static_assert((SIZE & (SIZE - 1)) == 0, "SIZE not binary exponent (2^n)"); \
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

//                                   error
#define ring_queue_push(_queue, _entry, ...)                                \
do {                                                                        \
  typeof(_queue) const q = _queue;                                          \
  typeof(_entry) e = _entry;                                                \
                                                                            \
  int wi = __atomic_load_n(&(q)->write_index, __ATOMIC_RELAXED);            \
  wi = next_index(wi, ring_queue_size(q));                                  \
                                                                            \
  if (wi == __atomic_load_n(&(q)->read_index, __ATOMIC_ACQUIRE))            \
    OPT_SET(1, __VA_ARGS__)                                                 \
  else                                                                      \
  {                                                                         \
    (q)->buffer[wi] = e;                                                    \
    __atomic_store_n(&(q)->write_index, wi, __ATOMIC_RELEASE);              \
    OPT_SET(0, __VA_ARGS__)                                                 \
  }                                                                         \
                                                                            \
} while(0)

//                                  error
#define ring_queue_eat(_queue, _entry, ...)                                 \
do {                                                                        \
  typeof(_queue) const q = _queue;                                          \
  typeof(_entry) e = _entry;                                                \
                                                                            \
  const int wi = __atomic_load_n(&(q)->write_index, __ATOMIC_ACQUIRE);      \
  int ri = __atomic_load_n(&(q)->read_index, __ATOMIC_RELAXED);             \
                                                                            \
  if (ri == wi)                                                             \
    OPT_SET(1, __VA_ARGS__)                                                 \
  else                                                                      \
  {                                                                         \
    ri = next_index(ri, ring_queue_size(q));                                \
    e = (q)->buffer[ri];                                                    \
    __atomic_store_n(&(q)->read_index, ri, __ATOMIC_RELEASE);               \
    OPT_SET(0, __VA_ARGS__)                                                 \
  }                                                                         \
                                                                            \
  _entry = e;                                                               \
} while(0)

#define ring_queue_eat_last(queue, entry, ...)                              \
do {                                                                        \
  const int wi = __atomic_load_n(&(queue)->write_index, __ATOMIC_ACQUIRE);  \
  int ri = __atomic_load_n(&(queue)->read_index, __ATOMIC_RELAXED);         \
                                                                            \
  if (ri == wi)                                                             \
    OPT_SET(1, __VA_ARGS__)                                                 \
  else                                                                      \
  {                                                                         \
    ri = wi;                                                                \
    entry = (queue)->buffer[ri];                                            \
    __atomic_store_n(&(queue)->read_index, ri, __ATOMIC_RELEASE);           \
    OPT_SET(0, __VA_ARGS__)                                                 \
  }                                                                         \
} while(0)
