#pragma once
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

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

static inline int shmem_unmap(void *addr, int size)
{
  int res;

  res = munmap(addr, size);
  debug_assert(res == 0, return -1);

  return 0;
}

static inline int shmem_unlink(const char* storage_id)
{
  int res;

  // shm_open cleanup
  res = shm_unlink(storage_id);
  debug_assert(res == 0, return -1);

  return 0;
}

static inline int shmem_unlink_all(const char *prefix)
{
  DIR *dir;
  struct dirent *ent;
  dir = opendir("/dev/shm");
  debug_assert(dir != NULL, return -1);

  while((ent = readdir(dir)) != NULL)
  {
    const char *dir_name = ent->d_name;
    int match = 1;

    for (int i=0; i < (int)strlen(prefix); ++i)
      if (dir_name[i] != prefix[i])
        match = 0;

    if (match)
    {
      debug("unlinking %s\n", dir_name + strlen(prefix) + 1);
      shmem_unlink(dir_name);
    }
  }
  return 0;
}

#ifdef __cplusplus
constexpr char shmem_default_prefix[] = "shmem";
template<typename DATA_STRUCTURE, bool WRITER, const char* PREFIX = shmem_default_prefix>
class shmem_data
{
  DATA_STRUCTURE *ds;
  typedef typename DATA_STRUCTURE::storage storage;

public:

  shmem_data() : ds(NULL) {}

  shmem_data(const char *id)
  {
    init(id);
  }

  int init(const char *id)
  {
    char handle[sizeof(PREFIX) + strlen(id) + 1];
    sprintf(handle, "%s_%s", PREFIX, id);

    ds = (DATA_STRUCTURE*)shmem_create(handle, sizeof(DATA_STRUCTURE));

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
    static_assert(WRITER, "not a writer");
    return ds->write(entry);
  }

  int read(storage& entry)
  {
    static_assert(!WRITER, "not a reader");
    return ds->read(entry);
  }

};
#endif
