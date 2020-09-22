#pragma once
#include "shmem.h"
#include "seq_lock.h"
#include "big_seq_lock.h"


const char ipc_shmem_prefix[] = "ipc";

#ifdef __cplusplus

//template<typename STORAGE>
//using ipc_writer = shmem_data<big_seq_lock<STORAGE, 64>, true>;
//
//template<typename STORAGE>
//using ipc_reader = shmem_data<big_seq_lock<STORAGE, 64>, false>;


template<typename STORAGE>
using ipc_writer = shmem_data<seq_lock<STORAGE>, true, ipc_shmem_prefix>;

template<typename STORAGE>
using ipc_reader = shmem_data<seq_lock<STORAGE>, false, ipc_shmem_prefix>;

#endif

int ipc_cleanup()
{
  shmem_unlink_all(ipc_shmem_prefix);
  return 0;
}
