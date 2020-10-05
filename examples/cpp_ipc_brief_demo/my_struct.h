
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

#define my_struct_inc(entry)    \
    do{                         \
      for (int i=0; i<50; ++i)  \
      {                         \
        ++(entry.data[i].ii);   \
        ++(entry.data[i].iii);  \
        ++(entry.data[i].iiii); \
      }                         \
    }while(0)
