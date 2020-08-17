#include <stdio.h>

#define ring_queue_def(STORAGE, SIZE)\
_Static_assert((SIZE & (SIZE - 1)) == 0, "SIZE not binary exponent (2^n)"); \
typedef struct                                                              \
{                                                                           \
  int read_index;                                                           \
  int write_index;                                                          \
  const int size;                                                           \
  STORAGE buffer[SIZE];                                                     \
}

#define ring_queue_inst(STORAGE, SIZE)  \
{                                       \
  .read_index=0,                        \
  .write_index=0,                       \
  .size=SIZE,                           \
  .buffer = {},                         \
}

// OPT_set(var) : var = 1;
// OPT_set() : ;
#define OPT_SET(set, ...) { __VA_ARGS__ __VA_OPT__(= set) ; }


#define next_index(index, size) (((index) + 1) & ((size)-1))

//                                   error
#define ring_queue_push(queue, entry, ...)                            \
do {                                                                  \
  int wi = __atomic_load_n(&(queue).write_index, __ATOMIC_RELAXED);   \
  int next = next_index((queue).write_index, (queue).size);           \
                                                                      \
  if (next == __atomic_load_n(&(queue).read_index, __ATOMIC_ACQUIRE)) \
    OPT_SET(1, __VA_ARGS__)                                           \
  else                                                                \
  {                                                                   \
    (queue).buffer[wi] = entry;                                       \
    __atomic_store_n(&(queue).write_index, next, __ATOMIC_RELEASE);   \
    OPT_SET(0, __VA_ARGS__)                                           \
  }                                                                   \
} while(0)

//                                  error
#define ring_queue_eat(queue, entry, ...)\
do {                                                                \
  int wi = __atomic_load_n(&(queue).write_index, __ATOMIC_ACQUIRE); \
  int ri = __atomic_load_n(&(queue).read_index, __ATOMIC_RELAXED);  \
                                                                    \
  if (ri == wi)                                                     \
    OPT_SET(1, __VA_ARGS__)                                         \
  else                                                              \
  {                                                                 \
    entry = (queue).buffer[ri];                                     \
    int next = next_index((queue).read_index, (queue).size);        \
    __atomic_store_n(&(queue).read_index, next, __ATOMIC_RELEASE);  \
    OPT_SET(0, __VA_ARGS__)                                         \
  }                                                                 \
} while(0)
