#define MX6_CACHELINE_BYTES 32
#define CACHELINE_BYTES MX6_CACHELINE_BYTES
// L1: https://developer.arm.com/documentation/ddi0388/f/Level-1-Memory-System/About-the-L1-memory-system
// L2: https://community.nxp.com/thread/510105

#define ring_queue_def(STORAGE, SIZE)\
_Static_assert((SIZE & (SIZE - 1)) == 0, "SIZE not binary exponent (2^n)"); \
typedef struct                                                              \
{                                                                           \
  const int size;                                                           \
  int read_index;                                                           \
  char padding[CACHELINE_BYTES - 2*sizeof(int)];                            \
  int write_index;                                                          \
  STORAGE buffer[SIZE];                                                     \
} __attribute__ ((aligned(CACHELINE_BYTES)))

#define ring_queue_inst(STORAGE, SIZE)  \
{                                       \
  .size=SIZE,                           \
  .read_index=0,                        \
  .write_index=0,                       \
  .buffer = {},                         \
}

// OPT_set(var) : var = 1;
// OPT_set() : ;
#define OPT_SET(set, ...) { __VA_ARGS__ __VA_OPT__(= set) ; }


#define next_index(index, size) (((index) + 1) & ((size)-1))

#define prev_index(index, size) (((index) - 1) & ((size)-1))

//                                   error
#define ring_queue_push(queue, entry, ...)                              \
do {                                                                    \
  int wi = __atomic_load_n(&(queue)->write_index, __ATOMIC_RELAXED);    \
  wi = next_index(wi, (queue)->size);                                   \
                                                                        \
  if (wi == __atomic_load_n(&(queue)->read_index, __ATOMIC_ACQUIRE))    \
    OPT_SET(1, __VA_ARGS__)                                             \
  else                                                                  \
  {                                                                     \
    (queue)->buffer[wi] = entry;                                        \
    __atomic_store_n(&(queue)->write_index, wi, __ATOMIC_RELEASE);      \
    OPT_SET(0, __VA_ARGS__)                                             \
  }                                                                     \
} while(0)

//                                  error
#define ring_queue_eat(queue, entry, ...)                                   \
do {                                                                        \
  const int wi = __atomic_load_n(&(queue)->write_index, __ATOMIC_ACQUIRE);  \
  int ri = __atomic_load_n(&(queue)->read_index, __ATOMIC_RELAXED);         \
                                                                            \
  if (ri == wi)                                                             \
    OPT_SET(1, __VA_ARGS__)                                                 \
  else                                                                      \
  {                                                                         \
    ri = next_index(ri, (queue)->size);                                     \
    entry = (queue)->buffer[ri];                                            \
    __atomic_store_n(&(queue)->read_index, ri, __ATOMIC_RELEASE);           \
    OPT_SET(0, __VA_ARGS__)                                                 \
  }                                                                         \
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
