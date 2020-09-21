#pragma once
#include <dirent.h>

#include "shmem.h"
#include "seq_lock.h"
#include "big_seq_lock.h"



#ifdef __cplusplus

//template<typename STORAGE>
//using ipc_writer = shmem_data<big_seq_lock<STORAGE, 64>, true>;
//
//template<typename STORAGE>
//using ipc_reader = shmem_data<big_seq_lock<STORAGE, 64>, false>;


constexpr char ipc_shmem_prefix[] = "ipc";

template<typename STORAGE>
using ipc_writer = shmem_data<seq_lock<STORAGE>, true, ipc_shmem_prefix>;

template<typename STORAGE>
using ipc_reader = shmem_data<seq_lock<STORAGE>, false, ipc_shmem_prefix>;



int ipc_startup(pid_t *pids, const char ** children, int n_children)
{
  for (int i=0; i < n_children; ++i)
  {
    pid_t pid = fork();

    debug_assert(pid != -1, return -1);

    if (pid == 0)
    {
      debug_error("child of %u with pid %u\n", getppid(), getpid());

      char * const child_argv[] = { (char*)children[i], NULL };
      execv(children[i], child_argv);
    }

    debug_error("parent of %u : %u\n", pid, getpid());
    pids[i] = pid;
  }

  return 0;
}

int ipc_cleanup()
{
  DIR *dir;
  struct dirent *ent;
  dir = opendir("/dev/shm");
  debug_assert(dir != NULL, return -1);

  while(ent = readdir(dir))
  {
    const char *dir_name = ent->d_name;
    bool match = true;

    for (int i=0; i < strlen(ipc_shmem_prefix); ++i)
      if (dir_name[i] != ipc_shmem_prefix[i])
        match = false;

    if (match)
    {
      debug("unlinking %s\n", dir_name + strlen(ipc_shmem_prefix) + 1);
      shmem_unlink(dir_name);
    }
  }
  return 0;
}

#endif
