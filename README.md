
# Inter process communication in Linux based on shared memory and lockless data structures

Header only.
Link with `-lrt` for mmap.

### Build
```
mkdir build
cd build
cmake ..
make
```

### Install
```
make install
```

### Build examples
```
make examples
```

### Minimal Demo
Full demo in [examples/cpp_ipc_demo/](examples/cpp_ipc_demo/).

#### Writer
```cpp
#include "ipc/ipc.h"
...
ipc_writer<my_struct> writer("my_topic");

while (1)
{
  // create / obtain entry
  my_struct entry = { .field = 1 };

  writer.write(entry);
}
```

#### Reader
```cpp
#include "ipc/ipc.h"
...
ipc_reader<my_struct> reader("my_topic");

while (1)
{
  my_struct entry;

  if (reader.read(entry) == 0)
  {
    // use entry
    int field = entry.field;
  }
}
```

#### Note on ring queues

When using the ring queue-based data structure exposed through `ipc_reader_rq`,
the reader should read all available elements to avoid exhaustion in case the writer is
quicker than the reader.

This can be achieved simply by changing the if statement above to a while loop:


```cpp
#include "ipc/ipc.h"
...
ipc_reader_rq<my_struct> reader("my_topic");

while (1)
{
  my_struct entry;

  while (reader.read(entry) == 0)
  {
    // use entry
    int field = entry.field;
  }
}
```

### Even more minimal example

This example uses the helper methods `ipc_write` / `ipc_read`, which contain static declared instances of the ipc objects,
so that the shared memory is allocated / mapped on the first call of `ipc_write` / `ipc_read` with a new entry type or id.
Full demo in [examples/cpp_ipc_brief_demo/](examples/cpp_ipc_brief_demo/).

#### Writer
```cpp
#include "ipc/ipc.h"
...
while (1)
{
  // create / obtain entry
  my_struct entry = { .field = 1 };

  ipc_write(entry, "my_topic");
}
```

#### Reader
```cpp
#include "ipc/ipc.h"
...
while (1)
{
  my_struct entry;

  if (ipc_read(entry, "my_topic") == 0)
  {
    // use entry
    int field = entry.field;
  }
}
```

### Debug output

A debugging interface is exposed in [include/ipc/debug.h](include/ipc/debug.h) .
It is used internally, but output is off by default.

Configure the debugging output by defining `debug_stdout` and `debug_stderr` to the wanted file descriptors before including `ipc.h`:

```cpp
#define debug_stdout stdout
#define debug_stderr stderr
#include "ipc/ipc.h"
```
