#pragma once
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "debug.h"

static inline void* ipc_create(const char* storage_id, int size)
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

static inline int ipc_destroy(const char* storage_id, int size)
{
  int res;

  // shm_open cleanup
  res = shm_unlink(storage_id);
  debug_assert(res == 0, return -1);

  return 0;
}

static inline void* ipc_open(const char* storage_id, int size)
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

static inline int ipc_unmap(void *addr, int size)
{
  int res;

  res = munmap(addr, size);
  debug_assert(res == 0, return -1);

  return 0;
}
