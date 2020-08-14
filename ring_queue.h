
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
  GET_RING_QUEUE(int, 64)*: GET_RING_QUEUE_METHOD(method, int, 64)((GET_RING_QUEUE(int, 64)*)queue COMMA(__VA_ARGS__))\
  ,GET_RING_QUEUE(char, 128)*: GET_RING_QUEUE_METHOD(method, char, 128)((GET_RING_QUEUE(char, 128)*)queue COMMA(__VA_ARGS__))\
)
