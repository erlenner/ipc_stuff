/*
seq_lock.h

Use calm_seq_lock.h if non-stop writing is needed

Inspired by:
http://www.1024cores.net/home/lock-free-algorithms/reader-writer-problem/improved-lock-free-seqlock
https://github.com/torvalds/linux/blob/master/include/vdso/helpers.h
https://github.com/rigtorp/Seqlock
*/

#pragma once
#include <assert.h> // for static_assert
#include "smp.h"


#define seq_lock_def(STORAGE)\
typedef struct                                                              \
{                                                                           \
  int seq;                                                                  \
  STORAGE entry;                                                            \
} __attribute__ ((aligned(CACHELINE_BYTES)))

#define seq_lock_init(queue)                                    \
do {                                                            \
  memset(queue, 0, sizeof(queue));                              \
} while (0)

// OPT_set(var) : var = 1;
// OPT_set() : ;
#define OPT_SET(set, ...) { __VA_ARGS__ __VA_OPT__(= set) ; }

/*
parameters:
_queue (in): pointer to object defined by seq_lock_def
_entry (in): instance of STORAGE object passed to seq_lock_def
*/
#define seq_lock_write(_queue, _entry)                                              \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
                                                                                    \
  int seq = (q)->seq;                                                               \
                                                                                    \
  (q)->seq = ++seq;                                                                 \
  smp_wmb();                                                                        \
  (q)->entry = _entry;                                                              \
  smp_wmb();                                                                        \
  (q)->seq = ++seq;                                                                 \
                                                                                    \
} while(0)

/*
parameters:
_queue (in): pointer to object defined by seq_lock_def
_entry (out): instance of STORAGE object passed to seq_lock_def
outout code (out, optional): integer representing the sequence number of the read entry
*/
#define seq_lock_read(_queue, _entry, .../*seq2*/)                                  \
do {                                                                                \
  typeof(_queue) const q = _queue;                                                  \
                                                                                    \
  while(1)                                                                          \
  {                                                                                 \
    int seq1 = (q)->seq;                                                            \
    if (seq1 & 1)                                                                   \
    {                                                                               \
      /*fprintf(stderr, "V");*/                                                     \
      cpu_relax();                                                                  \
      continue;                                                                     \
    }                                                                               \
                                                                                    \
    smp_rmb();                                                                      \
    _entry = (q)->entry;                                                            \
    smp_rmb();                                                                      \
                                                                                    \
    int seq2 = (q)->seq;                                                            \
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
