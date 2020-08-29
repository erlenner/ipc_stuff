#include <stdio.h>
#include "msg.h"

int main()
{
  msg_def(msg, int, a, 1, float, b, 1, char, c, 32);
  msg.a[0] = 5;
  msg.b[0] = 3.14;
  sprintf(msg.c, "hei");

  printf("%s: %d, %d, %f, %s\n", msg.id, msg.size, msg.a[0], msg.b[0], msg.c);
  printf("%s%s", msg_print(msg), "\n");

  return 0;
}
