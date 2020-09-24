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

#endif

int ipc_cleanup()
{
  return shmem_unlink_all(ipc_shmem_prefix);
}
