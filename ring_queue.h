#include <stdio.h>
#include "macros.h"

#define GET_RING_QUEUE(STORAGE, SIZE) CAT5(ring_queue, _, STORAGE, _, SIZE)

#define RING_QUEUE_DEF(STORAGE, SIZE)\
_Static_assert((SIZE & (SIZE - 1)) == 0, "SIZE not binary exponent (2^n)");\
typedef struct\
{\
  int read_index;\
  int write_index;\
  const int size;\
  STORAGE buffer[SIZE];\
}

#define RING_QUEUE_INST(STORAGE, SIZE)\
{\
  .read_index=0,\
  .write_index=0,\
  .size=SIZE,\
  .buffer = {},\
};

#define next_index(index, size) (((index) + 1) & ((size)-1))


// OPT_ERROR(error) : error = 1;
// OPT_ERROR() : ;
#define OPT_SET(set, ...) { __VA_ARGS__ __VA_OPT__(= set) ; }

//                                   error
#define RING_QUEUE_PUSH(queue, entry, ...)\
{\
  int wi = __atomic_load_n(&(queue).write_index, __ATOMIC_RELAXED);\
  int next = next_index((queue).write_index, (queue).size);\
\
  if (next == __atomic_load_n(&(queue).read_index, __ATOMIC_ACQUIRE))\
    OPT_SET(1, __VA_ARGS__)\
  else\
  {\
    (queue).buffer[wi] = entry;\
    __atomic_store_n(&(queue).write_index, next, __ATOMIC_RELEASE);\
    OPT_SET(0, __VA_ARGS__)\
  }\
}

//                                  error
#define RING_QUEUE_EAT(queue, entry, ...)\
{\
  int wi = __atomic_load_n(&(queue).write_index, __ATOMIC_ACQUIRE);\
  int ri = __atomic_load_n(&(queue).read_index, __ATOMIC_RELAXED);\
\
  if (ri == wi)\
    OPT_SET(1, __VA_ARGS__)\
  else\
  {\
    entry = (queue).buffer[ri];\
    int next = next_index((queue).read_index, (queue).size);\
    __atomic_store_n(&(queue).read_index, next, __ATOMIC_RELEASE);\
    OPT_SET(0, __VA_ARGS__)\
  }\
}
