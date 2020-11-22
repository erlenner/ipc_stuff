/*
seq_lock.h

Sequence lock: https://en.wikipedia.org/wiki/Seqlock, https://lwn.net/Articles/21355/

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
  smp_write_once((q)->seq, ++seq);                                                  \
  smp_wmb();                                                                        \
  (q)->entry = _entry;                                                              \
  smp_wmb();                                                                        \
  smp_write_once((q)->seq, ++seq);                                                  \
                                                                                    \
} while(0)

/*
parameters:
_lock (in): pointer to object defined by seq_lock_def
_entry (out): instance of STORAGE object passed to seq_lock_def
outout code (out, optional): integer representing the sequence number of the read entry
*/
#define seq_lock_read(_lock, _entry, .../*seq2*/)                                   \
do {                                                                                \
  typeof(_lock) const q = _lock;                                                    \
                                                                                    \
  int seq1 = smp_read_once((q)->seq);                                               \
                                                                                    \
  while(1)                                                                          \
  {                                                                                 \
    if (seq1 & 1)                                                                   \
    {                                                                               \
      cpu_relax();                                                                  \
      seq1 = smp_read_once((q)->seq);                                               \
      continue;                                                                     \
    }                                                                               \
                                                                                    \
    smp_rmb();                                                                      \
    _entry = (q)->entry;                                                            \
    smp_rmb();                                                                      \
                                                                                    \
    int seq2 = smp_read_once((q)->seq);                                             \
    if (seq2 == seq1)                                                               \
    {                                                                               \
      OPT_SET(seq2, __VA_ARGS__)                                                    \
      break;                                                                        \
    }                                                                               \
                                                                                    \
    seq1 = seq2;                                                                    \
  }                                                                                 \
                                                                                    \
} while(0)

#ifdef __cplusplus


struct seq_lock_data
{
  int last_seq;
};

template<typename STORAGE>
class seq_lock
{
  static_assert(std::is_nothrow_copy_assignable<STORAGE>::value);
  static_assert(std::is_trivially_copy_assignable<STORAGE>::value);

  int seq;
  STORAGE entry;

public:
  int write(const STORAGE& entry, seq_lock_data data = {0})
  {
    seq_lock_write(this, entry);
    return 0;
  }

  int read(STORAGE& entry, seq_lock_data& data)
  {
    int seq, ret;
    seq_lock_read(this, entry, seq);
    ret = (seq == data.last_seq);
    data.last_seq = seq;
    return ret;
  }

  // NOTE: This returns the sequence number
  int read(STORAGE& entry)
  {
    int seq;
    seq_lock_read(this, entry, seq);
    return seq;
  }

  typedef STORAGE storage;
  typedef seq_lock_data data;

} __attribute__ ((aligned(CACHELINE_BYTES)));

#endif
