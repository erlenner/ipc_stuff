#include <string.h>


//typedef struct
//{
//  char *buf;
//  int read_index;
//  int write_index;
//} ring_queue;

//#define RING_SIZE 1<<8
//#define RING_SIZE_MASK RING_SIZE-1

// https://github.com/torvalds/linux/blob/master/include/linux/circ_buf.h
// https://github.com/torvalds/linux/blob/master/Documentation/core-api/circular-buffers.rst
// https://github.com/torvalds/linux/blob/master/Documentation/memory-barriers.txt
///* Return count in buffer.  */
//#define cnt(read_index,write_index,ring_size) (((read_index) - (write_index)) & (ring_size-1))
//
///* Return space available, 0..RING_SIZE-1.  We always leave one free char
//   as a completely full buffer has read_index == write_index, which is the same as
//   empty.  */
//#define space(read_index,write_index) cnt((write_index),((read_index)+1),(ring_size))
//
///* Return count up to the end of the buffer.  Carefully avoid
//   accessing read_index and write_index more than once, so they can change
//   underneath us without returning inconsistent results.  */
//#define cnt_to_end(read_index,write_index) \
//  ({int end = (ring_size) - (write_index); \
//    int n = ((read_index) + end) & ((RING_SIZE)-1); \
//    n < end ? n : end;})
//
///* Return space available up to the end of the buffer.  */
//#define space_to_end(read_index,write_index) \
//  ({int end = (ring_size) - 1 - (read_index); \
//    int n = (end + (write_index)) & ((RING_SIZE)-1); \
//    n <= end ? n : end+1;})



// interface inspired by boost: https://github.com/boostorg/lockfree/blob/develop/include/boost/lockfree/spsc_queue.hpp

#define is_binary_exponent(n) ((n & (n-1)) == 0)

#define ring_define(type, size)\
_Static_assert (is_binary_exponent(size), "exponent is a binary exponent (2^n)");\
const int ring_size = size;\
type ring_buffer[size];\
int read_index;\
int write_index;

#define ring_init()\
{\
  read_index = write_index = 0;\
  /*memset(ring_buffer, 0, ring_size*sizeof(ring_buffer[0]));*/\
}

#define next_index(index, size) (((index) + 1) & ((size)-1))


#define cnt(head, tail, size) (((head) - (tail)) & ((size)-1))

#define read_available(read_index, write_index, size) cnt(write_index, read_index, size)

#define write_available(read_index, write_index, size) cnt(read_index, write_index+1, size)

#define ring_push(entry)\
  {\
    int wi = __atomic_load_n(&write_index, __ATOMIC_RELAXED);\
    int next = next_index(write_index, ring_size);\
    if (next != __atomic_load_n(&read_index, __ATOMIC_ACQUIRE))\
    {\
      ring_buffer[wi] = entry;\
      __atomic_store_n(&write_index, next, __ATOMIC_RELEASE);\
    }\
  }

#define ring_eat(entry)\
  {\
    int wi = __atomic_load_n(&write_index, __ATOMIC_ACQUIRE);\
    int ri = __atomic_load_n(&read_index, __ATOMIC_RELAXED);\
    if (ri == wi)\
      entry = -1;\
    else\
    {\
      entry = ring_buffer[ri];\
      int next = next_index(read_index, ring_size);\
      __atomic_store_n(&read_index, next, __ATOMIC_RELEASE);\
    }\
  }
