#include <stdio.h>
#include "msg.h"

int main()
{

  char buffer0[512];

  {
    msg_def(msg0, int, a, 1, float, b, 1, char, c, 32);
    msg0.a[0] = 5;
    msg0.b[0] = 3.14;
    sprintf(msg0.c, "hei");

    printf("%s %d: %d, %f, %s\n", msg0.id, msg0.size, msg0.a[0], msg0.b[0], msg0.c);
    //printf("%s%s", msg_print(msg0), "\n");

    memcpy(buffer0, &msg0, sizeof(msg0));
  }



  {
    msg_def(msg1, int, a, 1, char, c, 32, long, d, 1, long long, e, 1);
    msg1.a[0] = 19;
    sprintf(msg1.c, "hade");
    msg1.d[0] = 628;
    msg1.e[0] = 345345345;

    printf("%s %d: %d, %s, %ld, %lld\n", msg1.id, msg1.size, msg1.a[0], msg1.c, msg1.d[0], msg1.e[0]);
    //printf("%s%s", msg_print(msg1), "\n");

    char buffer1[512];
    memcpy(buffer1, buffer0, msg_header_size(msg1));

    if (msg_match(msg, buffer1))
    {
      printf("no match\n");
      return 1;
    }

    memcpy(buffer1 + msg_header_size(msg1), buffer0 + msg_header_size(msg1), msg_size(buffer) - msg_header_size(msg1));

    msg_parse(msg1, buffer)
  }

  return 0;
}
