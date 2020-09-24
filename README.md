
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

# Writer
```
ipc_writer<my_struct> writer("my_topic");
my_struct entry = {0};

while (1)
{

  // modify entry
  entry.field += 5;

  writer.write(entry);
}
```

# Reader
```
ipc_reader<my_struct> reader("my_topic");

while (run)
{
  my_struct entry;

  if (reader.read(entry) == 0)
  {
    debug("new entry:\t");
    my_struct_print(debug_plain, entry, "\n");
  }
}
```

Note: When using the ring queue based data structure, exposed through `ipc_reader_rq`,
the reader should read all available elements to avoid exhaustion in case the writer is
quicker than the reader.

This can be achieved simply by changing the if statement above to a while loop:


```
ipc_reader_rq<my_struct> reader("my_topic");

while (run)
{
  my_struct entry;

  while (reader.read(entry) == 0)
  {
    debug("new entry:\t");
    my_struct_print(debug_plain, entry, "\n");
  }
}
```
