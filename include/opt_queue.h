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


//#define smp_mb() atomic_thread_fence(memory_order_acq_rel);
//#define smp_wmb() smp_mb()
//#define smp_rmb() smp_mb()

#if __arm__
// https://github.com/torvalds/linux/blob/master/arch/arm/include/asm/barrier.h
#define dmb(option) __asm__ __volatile__ ("dmb " #option : : : "memory")
#define smp_mb() dmb(ish);
#define smp_wmb() dmb(ishst);
#define smp_rmb() smp_mb()
//#define smp_rmb() dmb(ishld); // armv8
#elif __x86_64__
#define barrier() __asm__ __volatile__("": : :"memory")
#define smp_mb() barrier()
#define smp_wmb() barrier()
#define smp_rmb() barrier()
#endif

#define opt_queue_def(STORAGE, SIZE)\
static_assert((SIZE & (SIZE - 1)) == 0, "SIZE not binary exponent (2^n)");  \
typedef struct                                                              \
{                                                                           \
  int write_index;                                                          \
  int seq;                                                                  \
  char padding[CACHELINE_BYTES - 2*sizeof(int)];                            \
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
  memset(queue, 0, sizeof(queue));                              \
} while (0)

// OPT_set(var) : var = 1;
// OPT_set() : ;
#define OPT_SET(set, ...) { __VA_ARGS__ __VA_OPT__(= set) ; }

#define next_index(index, size) (((index) + 1) & ((size)-1))

/*
parameters:
_queue (in): pointer to object defined by opt_queue_def
_entry (in): instance of STORAGE object passed to opt_queue_def
*/
#define opt_queue_write(_queue, _entry)                                             \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  int seq = (q)->seq;                                                               \
                                                                                    \
  int wi = atomic_load_explicit(&(q)->write_index, memory_order_acquire);           \
  wi = next_index(wi, opt_queue_size(q));                                           \
                                                                                    \
  (q)->buffer[wi].seq = ++seq;                                                      \
  smp_wmb();                                                                        \
  (q)->buffer[wi].entry = e;                                                        \
  smp_wmb();                                                                        \
  (q)->buffer[wi].seq = ++seq;                                                      \
                                                                                    \
  atomic_store_explicit(&(q)->write_index, wi, memory_order_release);               \
  /*smp_wmb();*/                                                                    \
  /*(q)->write_index = wi;*/                                                        \
                                                                                    \
  (q)->seq = seq;                                                                   \
} while(0)

/*
parameters:
_queue (in): pointer to object defined by opt_queue_def
_entry (out): instance of STORAGE object passed to opt_queue_def
outout code (out, optional): integer representing the sequence number of the read entry
*/
#define opt_queue_read(_queue, _entry, .../*output code*/)                          \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
  typeof(_entry) e = _entry;                                                        \
                                                                                    \
  while(1)                                                                          \
  {                                                                                 \
    /*static int total = 0;*/                                                       \
    /*++total;*/                                                                    \
    const int wi = atomic_load_explicit(&(q)->write_index, memory_order_acquire);   \
    int seq1 = (q)->buffer[wi].seq;                                                 \
    if (seq1 & 1)                                                                   \
    {                                                                               \
      /*fprintf(stderr, "V");*/                                                     \
      continue;                                                                     \
    }                                                                               \
                                                                                    \
    smp_rmb();                                                                      \
    e = (q)->buffer[wi].entry;                                                      \
    smp_rmb();                                                                      \
                                                                                    \
    int seq2 = (q)->buffer[wi].seq;                                                 \
    if (seq2 == seq1)                                                               \
    {                                                                               \
      _entry = e;                                                                   \
      OPT_SET(seq2, __VA_ARGS__)                                                    \
      break;                                                                        \
    }                                                                               \
    /*static int fail = 0;*/                                                        \
    /*fprintf(stderr, "\nFAILED: %d %d %f\n", ++fail, total, (total > 0) ? ((float)fail / (float)total) : 0);*/ \
  }                                                                                 \
                                                                                    \
  /*fprintf(stderr, "i");*/                                                         \
  /*printf("ok %d\n", _entry);*/                                                    \
} while(0)
