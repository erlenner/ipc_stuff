#pragma once
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "debug.h"

static inline void* shmem_create(const char* storage_id, int size)
{
  int res;
  int fd;
  void *addr;

  // memfd_create
  fd = shm_open(storage_id, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  debug_assert(fd != -1, return NULL);

  res = ftruncate(fd, size);
  debug_assert(res == 0, return NULL);

  addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  debug_assert(addr != MAP_FAILED, return NULL);

  return addr;
}

static inline void* shmem_open(const char* storage_id, int size)
{
  int res;
  int fd;
  void *addr;

  fd = shm_open(storage_id, O_RDWR, S_IRUSR | S_IWUSR);
  debug_assert(fd != -1, return NULL);

  res = ftruncate(fd, size);
  debug_assert(res == 0, return NULL);

  addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  debug_assert(addr != MAP_FAILED, return NULL);

  return addr;
}

static inline int shmem_destroy(const char* storage_id)
{
  int res;

  // shm_open cleanup
  res = shm_unlink(storage_id);
  debug_assert(res == 0, return -1);

  return 0;
}

static inline int shmem_unmap(void *addr, int size)
{
  int res;

  res = munmap(addr, size);
  debug_assert(res == 0, return -1);

  return 0;
}

#ifdef __cplusplus
template<typename DATA_STRUCTURE, bool CREATE>
class shmem_data
{
  DATA_STRUCTURE *ds;
  typedef typename DATA_STRUCTURE::storage storage;

public:

  shmem_data() : ds(NULL) {}

  shmem_data(const char* id)
  {
    init(id);
  }

  int init(const char* id)
  {
    if (CREATE)
      ds = (DATA_STRUCTURE*)shmem_create(id, sizeof(DATA_STRUCTURE));
    else
      ds = (DATA_STRUCTURE*)shmem_open(id, sizeof(DATA_STRUCTURE));
    return (ds == NULL) ? 1 : 0;
  }

  ~shmem_data()
  {
    shmem_unmap((void*)ds, sizeof(DATA_STRUCTURE));

    //if (CREATE)
    //  shm_destroy(id);
  }

  int write(const storage& entry)
  {
    return ds->write(entry);
  }

  int read(storage& entry)
  {
    return ds->read(entry);
  }

};
#endif
