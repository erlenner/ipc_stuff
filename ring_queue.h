
#define RING_STORAGE int
#define RING_SIZE 64
#include "ring_queue_impl.h"
#undef RING_STORAGE
#undef RING_SIZE

#define RING_STORAGE char
#define RING_SIZE 128
#include "ring_queue_impl.h"
#undef RING_STORAGE
#undef RING_SIZE

#define RING_QUEUE(method, queue, ...) _Generic((queue),\
  GET_RING_QUEUE_GENERIC_ENTRY(method, queue, int, 64, __VA_ARGS__)\
  , GET_RING_QUEUE_GENERIC_ENTRY(method, queue, char, 128, __VA_ARGS__)\
)
