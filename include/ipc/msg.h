#include <string.h>
#include <assert.h>

template<typename T>
class msg
{
  struct h {
    static const int id_size = 8;
    char id[id_size];
    int size;
    h(const char* id) : size(sizeof(T))
    {
      assert(strlen(id) < id_size);
      strcpy(this->id, id);
    }
  } header;
  T data;

public:

  msg(const T& data, const char* id)
  : header(id)
  , data(data)
  {
  }


  void pack(char* buffer)
  {
    memcpy(buffer, this, sizeof(this));
  }



  void unpack(char* buffer)
  {
    memcpy(this, buffer, sizeof(this));
  }
} __attribute__((__packed__));




