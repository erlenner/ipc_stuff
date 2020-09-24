
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

  int seq = reader.read(entry);

  static int last_seq = 0;

  if (seq != last_seq)
  {
    last_seq = seq;

    // use entry
  }
}
```
