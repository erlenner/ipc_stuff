#include <string.h>
#include <assert.h>
#include "macro.h"

#define ID_SIZE 8
#define SIZE_TYPE int


//#define msg_def(_id, _data)                   \
//struct __attribute__((__packed__)) {          \
//  struct {                                    \
//    char id[ID_SIZE];                         \
//    int size;                                 \
//  } header;                                   \
//  _data data;
// } _id = { STRINGIFY(_id), sizeof(_id), }
//
//#define msg_header_size(msg) (sizeof(msg.header))





//#define msg_header_size (ID_SIZE * sizeof(char) + sizeof(SIZE_TYPE))
//
//#define msg_size(object) (msg_header_size + sizeof(object))

//#define msg_pack(object, buffer, buffer_size)       \
//*(struct __attribute__((__packed__)) {            \
//  struct {                                        \
//    char id[ID_SIZE] /*= STRINGIFY(object)*/;     \
//    int size /*= msg_size(object)*/;              \
//  } header;                                       \
//  typeof(object) data;                            \
//}&)buffer = { STRINGIFY(_id), sizeof(_id), object}\
//
//#define msg_pack_static(object, buffer)             \
//do{                                                 \
//static_assert(sizeof(buffer) >= sizeof(object) + msg_header_size)\
//msg_pach(object, buffer, sizeof(buffer))           \
//} while(0)


template<typename T, const char* ID>
class msg
{
  struct h {
    static_assert(strlen(ID) < ID_SIZE);
    char id[ID_SIZE];
    int size;
    h() : size(sizeof(T)){ strcpy(id, ID); }
  } header;
  T data;

public:

  msg(const T& data)
  : data(data)
  {}


  void pack(char* buffer)
  {
    
  }



  void unpack(char* buffer)
  {
  }
} __attribute__((__packed__));




