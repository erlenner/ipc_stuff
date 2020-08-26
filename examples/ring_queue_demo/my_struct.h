
typedef struct
{
  struct
  {
    unsigned int ii;
    char c;
    unsigned long l;
    char cc;
    unsigned long long ll;
  } data[50];
} my_struct;

#define my_struct_print(print, s, ...) print("%u\t%lu\t%llu" __VA_ARGS__ , s.data[0].ii, s.data[0].l, s.data[0].ll)
