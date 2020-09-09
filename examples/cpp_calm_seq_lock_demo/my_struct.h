
typedef struct
{
  struct
  {
    int ii;

    char c;
    char p1[23];

    int iii;

    char p2[54];

    int iiii;

  } data[50];
} my_struct;

#define my_struct_print(print, s, ...) print("%d\t%d\t%d" __VA_ARGS__ , s.data[0].ii, s.data[0].iii, s.data[0].iiii)
