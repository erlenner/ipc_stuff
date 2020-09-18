#include "shmem.h"
#include "calm_seq_lock.h"



#ifdef __cplusplus

template<typename STORAGE, int SIZE>
using ipc_sender_adj = shmem_data<calm_seq_lock<STORAGE, SIZE>, true>;

template<typename STORAGE, int SIZE>
using ipc_receiver_adj = shmem_data<calm_seq_lock<STORAGE, SIZE>, false>;


template <typename STORAGE>
using ipc_sender = ipc_sender_adj<STORAGE, 64>;

template <typename STORAGE>
using ipc_receiver = ipc_receiver_adj<STORAGE, 64>;

#endif
