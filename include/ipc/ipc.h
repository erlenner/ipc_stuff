#include "shmem.h"
#include "seq_lock.h"
#include "big_seq_lock.h"



#ifdef __cplusplus

//template<typename STORAGE>
//using ipc_writer = shmem_data<big_seq_lock<STORAGE, 64>, true>;
//
//template<typename STORAGE>
//using ipc_reader = shmem_data<big_seq_lock<STORAGE, 64>, false>;



template<typename STORAGE>
using ipc_writer = shmem_data<seq_lock<STORAGE>, true>;

template<typename STORAGE>
using ipc_reader = shmem_data<seq_lock<STORAGE>, false>;

#endif
