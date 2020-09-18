#include <stdio.h>
#include "ipc/old_msg.h"

int main()
{

  char buffer0[512];

  {
    old_msg_def(old_msg0, int, a, 1, float, b, 1, char, c, 32);
    old_msg0.a[0] = 5;
    old_msg0.b[0] = 3.14;
    sprintf(old_msg0.c, "hei");

    printf("%s %d: %d, %f, %s\n", old_msg0.id, old_msg0.size, old_msg0.a[0], old_msg0.b[0], old_msg0.c);
    //printf("%s%s", old_msg_print(old_msg0), "\n");

    memcpy(buffer0, &old_msg0, sizeof(old_msg0));
  }



  {
    old_msg_def(old_msg1, int, a, 1, char, c, 32, long, d, 1, long long, e, 1);
    old_msg1.a[0] = 19;
    sprintf(old_msg1.c, "hade");
    old_msg1.d[0] = 628;
    old_msg1.e[0] = 345345345;

    printf("%s %d: %d, %s, %ld, %lld\n", old_msg1.id, old_msg1.size, old_msg1.a[0], old_msg1.c, old_msg1.d[0], old_msg1.e[0]);
    //printf("%s%s", old_msg_print(old_msg1), "\n");

    char buffer1[512];
    memcpy(buffer1, buffer0, old_msg_header_size(old_msg1));

    if (old_msg_match(old_msg, buffer1))
    {
      printf("no match\n");
      return 1;
    }

    memcpy(buffer1 + old_msg_header_size(old_msg1), buffer0 + old_msg_header_size(old_msg1), old_msg_size(buffer) - old_msg_header_size(old_msg1));

    old_msg_parse(old_msg1, buffer)
  }

  return 0;
}
