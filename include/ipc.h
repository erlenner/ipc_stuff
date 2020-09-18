#include "shmem.h"
#include "seq_lock.h"
#include "calm_seq_lock.h"



#ifdef __cplusplus

//template<typename STORAGE>
//using ipc_sender = shmem_data<calm_seq_lock<STORAGE, 64>, true>;
//
//template<typename STORAGE>
//using ipc_receiver = shmem_data<calm_seq_lock<STORAGE, 64>, false>;



template<typename STORAGE>
using ipc_sender = shmem_data<seq_lock<STORAGE>, true>;

template<typename STORAGE>
using ipc_receiver = shmem_data<seq_lock<STORAGE>, false>;

#endif
