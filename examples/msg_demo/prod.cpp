#include <stdio.h>
#include "ipc/msg.h"

int main()
{

  char buffer0[512];

  {
    struct
    {
      int a;
      float b;
      char c[32];
    } msg0;
    msg0.a = 5;
    msg0.b = 3.14;
    sprintf(msg0.c, "hei");

    msg<typeof(msg0)> m(msg0, "msg0");

    //printf("%s %d: %d, %f, %s\n", msg0.id, msg0.size, msg0.a, msg0.b, msg0.c);
    //printf("%s%s", msg_print(msg0), "\n");

    //msg_pack(msg0, buffer0, sizeof(buffer0));
    //memcpy(buffer0, &msg0, sizeof(msg0));
  }



  //{
  //  msg_def(msg1,
  //    int a;
  //    char c[32];
  //    long d;
  //    long long e;
  //  );
  //  msg1.a = 19;
  //  sprintf(msg1.c, "hade");
  //  msg1.d = 628;
  //  msg1.e = 345345345;

  //  printf("%s %d: %d, %s, %ld, %lld\n", msg1.id, msg1.size, msg1.a, msg1.c, msg1.d, msg1.e);
  //  //printf("%s%s", msg_print(msg1), "\n");

  //  char buffer1[512];
  //  int read = 0;
  //  memcpy(buffer1, buffer0 + read, sizeof(msg1.header));
  //  memcpy((void*)&msg1, buffer1, sizeof(msg1.header));


  //  memcpy(buffer1, buffer0 + read, sizeof(msg1.data) - read);
  //  memcpy((void*)&msg1 + read, buffer1, sizeof(msg1) - read);

  //  memcpy(buffer1 + msg_header_size(msg1), buffer0 + msg_header_size(msg1), msg_size(buffer) - msg_header_size(msg1));

  //  msg_parse(msg1, buffer)
  //}

  return 0;
}
