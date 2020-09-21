#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ipc/macro.h>

#define debug_f(fd, ...) fprintf(fd, "[" __FILE__ "\tline " STRINGIFY(__LINE__) "] " __VA_ARGS__)

#define debug(...) debug_f(stdout, __VA_ARGS__)

#define error(...) fprintf(stderr, __VA_ARGS__);
#define debug_error(...) debug_f(stderr, __VA_ARGS__)

#define debug_assert(cond, ...) { if (!(cond)) { error("[%s\tline %5d] assertion failed: (" STRINGIFY(cond) ") errno: ", __FILE__, __LINE__); error("%s\n", strerror(errno)); __VA_ARGS__; } }

#define debug_assert_v(cond, ...) { if (!(cond)) { error("[%s\tline %5d] ", __FILE__, __LINE__); error(__VA_ARGS__); error("%s\n", strerror(errno)); /*exit(-1);*/ } }

#define debug_abort(...) { debug_assert_v(false, __VA_ARGS__); exit(1); }
