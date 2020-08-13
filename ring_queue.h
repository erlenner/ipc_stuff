#define RING_STORAGE int
#define RING_SIZE 64
#include "ring_queue_impl.h"
#undef RING_SIZE
#undef RING_STORAGE

#define RING_STORAGE char
#define RING_SIZE 128
#include "ring_queue_impl.h"
#undef RING_SIZE
#undef RING_STORAGE
