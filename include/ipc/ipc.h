#pragma once
#include "shmem.h"

//                                    echo ipc | sha1sum
const char ipc_shmem_prefix[] = "ipc" "1ef42bc4e0bbfeb0ac34bc3642732768cf6f77b7";

#ifdef __cplusplus


// Standard seqlock / sequence lock (https://en.wikipedia.org/wiki/Seqlock)
// Supports single producer, multiple consumers
// Reader always gets the most recent entry, potentially skipping older entries
// Consider using big_seq_lock / ipc_writer_bsl if write frequency exceeds 10kHz
#include "seq_lock.h"
template<typename STORAGE>
using ipc_writer_sl = shmem_data<seq_lock<STORAGE>, true, ipc_shmem_prefix>;
template<typename STORAGE>
using ipc_reader_sl = shmem_data<seq_lock<STORAGE>, false, ipc_shmem_prefix>;


// Modified seqlock from http://www.1024cores.net/home/lock-free-algorithms/reader-writer-problem/improved-lock-free-seqlock
// Exchangable with the standard seq_lock, ipc_writer_sl
// Uses more space in exchange for less chance of collisions / retries in reader
#include "big_seq_lock.h"
template<typename STORAGE>
using ipc_writer_bsl = shmem_data<big_seq_lock<STORAGE, 64>, true, ipc_shmem_prefix>;

template<typename STORAGE>
using ipc_reader_bsl = shmem_data<big_seq_lock<STORAGE, 64>, false, ipc_shmem_prefix>;


// Single producer, single consumer FIFO queue
// Uses a fixed size ring buffer
// In the case of buffer exhaustion, new entries are lost
// Readers should always keep reading buffers until there are none left, in order to mitigate buffer exhaustion
#include "ring_queue.h"
template<typename STORAGE>
using ipc_writer_rq = shmem_data<ring_queue<STORAGE, 64>, true, ipc_shmem_prefix>;
template<typename STORAGE>
using ipc_reader_rq = shmem_data<ring_queue<STORAGE, 64>, false, ipc_shmem_prefix>;



// default implementation
template<typename STORAGE>
using ipc_writer = ipc_writer_sl<STORAGE>;
template<typename STORAGE>
using ipc_reader = ipc_reader_sl<STORAGE>;


// helpers with static ipc instances
#include "hash.h"
#define ipc_read(entry, id) ipc_static_read<typeof(entry), id ## _hash>(entry, id)
#define ipc_write(entry, id) ipc_static_write<typeof(entry), id ## _hash>(entry, id)

#include <sys/time.h>
inline double ms()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);

  double ms = (double)(tp.tv_sec)*1000 + (double)(tp.tv_usec)/1000;
  return ms;
}

inline double toc(double* start)
{
  double now = ms();
  double dt = now - *start;
  *start = now;
  printf("toc: %f\n", dt);
  return dt;
}

template<typename STORAGE, int id_hash>
int ipc_static_write(const STORAGE& entry, const char *id)
{
  static ipc_writer<STORAGE> writer(id);

  return writer.write(entry);
}

template<typename STORAGE, int id_hash>
int ipc_static_read(STORAGE& entry, const char *id)
{
  double now = ms();
  static ipc_reader<STORAGE> reader(id);
  toc(&now);

  return reader.read(entry);
}


#endif

int ipc_cleanup()
{
  return shmem_unlink_all(ipc_shmem_prefix);
}
