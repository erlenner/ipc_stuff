
#ifndef RING_SIZE
_Static_assert(0, "RING_SIZE not defined")
#endif
_Static_assert((RING_SIZE & (RING_SIZE - 1)) == 0, "RING_SIZE not binary exponent (2^n)");

#ifndef RING_STORAGE
_Static_assert(0, "RING_STORAGE not defined")
#endif

typedef struct
{
  int read_index;
  int write_index;
  RING_STORAGE buffer[RING_SIZE];
} ring_queue;

// interface inspired by boost: https://github.com/boostorg/lockfree/blob/develop/include/boost/lockfree/spsc_queue.hpp

static inline void init(ring_queue *queue)
{
  queue->read_index = queue->write_index = 0;
}

#define next_index(index, size) (((index) + 1) & ((size)-1))

static inline int push(ring_queue *queue, const RING_STORAGE *entry)
{
  int wi = __atomic_load_n(&queue->write_index, __ATOMIC_RELAXED);
  int next = next_index(queue->write_index, RING_SIZE);

  if (next == __atomic_load_n(&queue->read_index, __ATOMIC_ACQUIRE))
    return 1;

  queue->buffer[wi] = *entry;
  __atomic_store_n(&queue->write_index, next, __ATOMIC_RELEASE);
  return 0;
}

static inline int eat(ring_queue *queue, RING_STORAGE *entry)
{
  int wi = __atomic_load_n(&queue->write_index, __ATOMIC_ACQUIRE);
  int ri = __atomic_load_n(&queue->read_index, __ATOMIC_RELAXED);

  if (ri == wi)
    return 1;

  *entry = queue->buffer[ri];
  int next = next_index(queue->read_index, RING_SIZE);
  __atomic_store_n(&queue->read_index, next, __ATOMIC_RELEASE);
  return 0;
}
