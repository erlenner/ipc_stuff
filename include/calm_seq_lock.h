/*
Optimistic Queue

Inspired by:
http://www.1024cores.net/home/lock-free-algorithms/reader-writer-problem/improved-lock-free-seqlock
https://github.com/torvalds/linux/blob/master/include/vdso/helpers.h
https://github.com/rigtorp/Seqlock
*/

#pragma once
#include <assert.h> // for static_assert
#include "smp.h"


#define calm_seq_lock_def(STORAGE, SIZE)\
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


#define calm_seq_lock_size(queue) (sizeof((queue)->buffer) / sizeof((queue)->buffer[0]))

#define calm_seq_lock_init(queue)                                   \
do {                                                            \
  memset(queue, 0, sizeof(queue));                              \
} while (0)

// OPT_set(var) : var = 1;
// OPT_set() : ;
#define OPT_SET(set, ...) { __VA_ARGS__ __VA_OPT__(= set) ; }

#define next_index(index, size) (((index) + 1) & ((size)-1))

/*
parameters:
_queue (in): pointer to object defined by calm_seq_lock_def
_entry (in): instance of STORAGE object passed to calm_seq_lock_def
*/
#define calm_seq_lock_write(_queue, _entry)                                             \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
                                                                                    \
  int seq = (q)->seq;                                                               \
                                                                                    \
  int wi = smp_load_acquire(q->write_index);                                        \
  wi = next_index(wi, calm_seq_lock_size(q));                                           \
                                                                                    \
  (q)->buffer[wi].seq = ++seq;                                                      \
  smp_wmb();                                                                        \
  (q)->buffer[wi].entry = _entry;                                                   \
  smp_wmb();                                                                        \
  (q)->buffer[wi].seq = ++seq;                                                      \
                                                                                    \
  smp_store_release(q->write_index, wi);                                            \
  /*smp_wmb();*/                                                                    \
  /*(q)->write_index = wi;*/                                                        \
                                                                                    \
  (q)->seq = seq;                                                                   \
} while(0)

/*
parameters:
_queue (in): pointer to object defined by calm_seq_lock_def
_entry (out): instance of STORAGE object passed to calm_seq_lock_def
outout code (out, optional): integer representing the sequence number of the read entry
*/
#define calm_seq_lock_read(_queue, _entry, .../*output code*/)                          \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
                                                                                    \
  while(1)                                                                          \
  {                                                                                 \
    /*static int total = 0;*/                                                       \
    /*++total;*/                                                                    \
    const int wi = smp_load_acquire(q->write_index);                                \
    int seq1 = (q)->buffer[wi].seq;                                                 \
    if (seq1 & 1)                                                                   \
    {                                                                               \
      /*fprintf(stderr, "V");*/                                                     \
      continue;                                                                     \
    }                                                                               \
                                                                                    \
    smp_rmb();                                                                      \
    _entry = (q)->buffer[wi].entry;                                                 \
    smp_rmb();                                                                      \
                                                                                    \
    int seq2 = (q)->buffer[wi].seq;                                                 \
    if (seq2 == seq1)                                                               \
    {                                                                               \
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
