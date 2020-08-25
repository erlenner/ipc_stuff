/*
Optimistic Queue

Inspired by:
http://www.1024cores.net/home/lock-free-algorithms/reader-writer-problem/improved-lock-free-seqlock
https://github.com/torvalds/linux/blob/master/include/vdso/helpers.h
https://github.com/rigtorp/Seqlock
*/

#pragma once
#include <assert.h> // for static_assert
#include <stdatomic.h>

#define MX6_CACHELINE_BYTES 32
#define CACHELINE_BYTES MX6_CACHELINE_BYTES
// L1: https://developer.arm.com/documentation/ddi0388/f/Level-1-Memory-System/About-the-L1-memory-system
// L2: https://community.nxp.com/thread/510105


#if __arm__
// https://github.com/torvalds/linux/blob/master/arch/arm/include/asm/barrier.h
#define dmb(option) __asm__ __volatile__ ("dmb " #option : : : "memory")
#define smp_mb() dmb(ish);
#define smp_wmb() dmb(ishst);
#define smp_rmb() smp_mb()
#elif __x86_64__
#define barrier() __asm__ __volatile__("": : :"memory")
#define smp_mb() barrier()
#define smp_wmb() barrier()
#define smp_rmb() barrier()
#endif

//#define __scalar_type_to_expr_cases(type)       \
//    unsigned type:  (unsigned type)0,     \
//    signed type:  (signed type)0
//
//#define __unqual_scalar_typeof(x) typeof(       \
//    _Generic((x),           \
//       char:  (char)0,        \
//       __scalar_type_to_expr_cases(char),   \
//       __scalar_type_to_expr_cases(short),    \
//       __scalar_type_to_expr_cases(int),    \
//       __scalar_type_to_expr_cases(long),   \
//       __scalar_type_to_expr_cases(long long),  \
//       default: (x)))
//
//#define compiletime_assert_rwonce_type(t)         \
//  static_assert((sizeof(t) == sizeof(int)) || (sizeof(t) == sizeof(long)) || (sizeof(t) == sizeof(long long)),  \
//    "Unsupported access size for {READ,WRITE}_ONCE().")
//#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
//#ifndef __READ_ONCE
//#define __READ_ONCE(x)  (*(const volatile __unqual_scalar_typeof(x) *)&(x))
//#endif
//#define READ_ONCE(x)              \
//({                  \
//  compiletime_assert_rwonce_type(x);        \
//  __READ_ONCE(x);             \
//})
//#define __WRITE_ONCE(x, val)            \
//do {                  \
//  *(volatile typeof(x) *)&(x) = (val);        \
//} while (0)
//#define WRITE_ONCE(x, val)            \
//do {                  \
//  compiletime_assert_rwonce_type(x);        \
//  __WRITE_ONCE(x, val);           \
//} while (0)

#define opt_queue_def(STORAGE, SIZE)\
static_assert((SIZE & (SIZE - 1)) == 0, "SIZE not binary exponent (2^n)");  \
typedef struct                                                              \
{                                                                           \
  int write_index;                                                          \
                                                                            \
  struct                                                                    \
  {                                                                         \
    int seq;                                                                \
    STORAGE entry;                                                          \
  } buffer[SIZE];                                                           \
                                                                            \
} __attribute__ ((aligned(CACHELINE_BYTES)))


#define opt_queue_size(queue) (sizeof((queue)->buffer) / sizeof((queue)->buffer[0]))

#define opt_queue_init(queue)                                   \
do {                                                            \
  (queue)->write_index = 0;                                     \
} while (0)


#define next_index(index, size) (((index) + 1) & ((size)-1))

#define opt_queue_write(_queue, _entry)                                             \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  int wi = atomic_load_explicit(&(q)->write_index, memory_order_acquire);           \
  wi = next_index(wi, opt_queue_size(q));                                           \
                                                                                    \
  static int seq = 0;                                                               \
  atomic_store_explicit(&(q)->buffer[wi].seq, ++seq, memory_order_relaxed);         \
  smp_wmb();                                                                        \
  (q)->buffer[wi].entry = e;                                                        \
  smp_wmb();                                                                        \
  atomic_store_explicit(&(q)->buffer[wi].seq, ++seq, memory_order_relaxed);         \
                                                                                    \
  atomic_store_explicit(&(q)->write_index, wi, memory_order_release);               \
                                                                                    \
} while(0)

#define opt_queue_read(_queue, _entry)                                              \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  while(1)                                                                          \
  {                                                                                 \
    /*static int total = 0;*/                                                       \
    /*++total;*/                                                                    \
    const int wi = atomic_load_explicit(&(q)->write_index, memory_order_acquire);   \
    int seq = atomic_load_explicit(&(q)->buffer[wi].seq, memory_order_relaxed);     \
    if (seq & 1)                                                                    \
    {                                                                               \
      fprintf(stderr, "V");                                                         \
      continue;                                                                     \
    }                                                                               \
                                                                                    \
    smp_mb();                                                                       \
    e = (q)->buffer[wi].entry;                                                      \
    smp_mb();                                                                       \
                                                                                    \
    int seq2 = atomic_load_explicit(&(q)->buffer[wi].seq, memory_order_relaxed);    \
    if (seq2 == seq)                                                                \
      break;                                                                        \
    /*static int fail = 0;*/                                                        \
    /*fprintf(stderr, "\nFAILED: %d %d %f\n", ++fail, total, (total > 0) ? ((float)fail / (float)total) : 0);*/ \
  }                                                                                 \
                                                                                    \
  _entry = e;                                                                       \
  /*fprintf(stderr, "i");*/                                                         \
  /*printf("ok %d\n", _entry);*/                                                    \
} while(0)
