#pragma once
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "debug.h"

#define STORAGE_ID "/ipc_test"

static inline void* ipc_create(int size)
{
  int res;
  int fd;
  void *addr;

  // memfd_create
  fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  debug_assert(fd != -1, return NULL);

  res = ftruncate(fd, size);
  debug_assert(res == 0, return NULL);

  addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  debug_assert(addr != MAP_FAILED, return NULL);

  return addr;
}

static inline int ipc_destroy(void *addr, int size)
{
  int res;

  // mmap cleanup
  res = munmap(addr, size);
  debug_assert(res == 0, return -1);

  // shm_open cleanup
  res = shm_unlink(STORAGE_ID);
  debug_assert(res == 0, return -1);

  return 0;
}

static inline void* ipc_open(int size)
{
  int res;
  int fd;
  void *addr;

  fd = shm_open(STORAGE_ID, O_RDWR, S_IRUSR | S_IWUSR);
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
