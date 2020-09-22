#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ipc/macro.h>

#define debug_f(fd, ...) fprintf(fd, "[" __FILE__ " line\t" STRINGIFY(__LINE__) "] " __VA_ARGS__)

#define debug(...) debug_f(stdout, __VA_ARGS__)

#define error(...) fprintf(stderr, __VA_ARGS__);
#define debug_error(...) debug_f(stderr, __VA_ARGS__)

#define debug_assert(cond, ...) do { if (!(cond)) { debug_error("assertion failed: (" STRINGIFY(cond) ") errno: "); error("%s\n", strerror(errno)); __VA_ARGS__; } } while(0)

#define debug_assert_v(cond, ...) do { if (!(cond)) { debug_error(__VA_ARGS__); error("%s\n", strerror(errno)); /*exit(-1);*/ } } while(0)

#define debug_abort(...) { debug_assert_v(false, __VA_ARGS__); exit(1); }
