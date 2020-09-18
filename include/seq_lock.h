/*
seq_lock.h

Use big_seq_lock.h if non-stop writing is needed

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

#define seq_lock_init(lock)                                    \
do {                                                            \
  memset(lock, 0, sizeof(lock));                              \
} while (0)

// OPT_set(var) : var = 1;
// OPT_set() : ;
#define OPT_SET(set, ...) { __VA_ARGS__ __VA_OPT__(= set) ; }

/*
parameters:
_lock (in): pointer to object defined by seq_lock_def
_entry (in): instance of STORAGE object passed to seq_lock_def
*/
#define seq_lock_write(_lock, _entry)                                              \
do {                                                                                \
  typeof(_lock) const q = _lock;                                                  \
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
_lock (in): pointer to object defined by seq_lock_def
_entry (out): instance of STORAGE object passed to seq_lock_def
outout code (out, optional): integer representing the sequence number of the read entry
*/
#define seq_lock_read(_lock, _entry, .../*seq2*/)                                  \
do {                                                                                \
  typeof(_lock) const q = _lock;                                                  \
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

#ifdef __cplusplus

template<typename STORAGE>
class seq_lock
{
  int seq;
  STORAGE entry;

public:
  int write(const STORAGE& entry)
  {
    seq_lock_write(this, entry);
    return 0;
  }

  int read(STORAGE& entry)
  {
    int seq;
    seq_lock_read(this, entry, seq);
    return seq;
  }

  typedef STORAGE storage;

} __attribute__ ((aligned(CACHELINE_BYTES)));

#endif
