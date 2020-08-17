#include <stdio.h>
#include <errno.h>
#include <string.h>

#define error(...) fprintf(stderr, __VA_ARGS__);

#define debug(...) { printf("[%s\tline %5d] ", __FILE__, __LINE__); printf(__VA_ARGS__); }

//#define debug_assert(cond, ...) { if (!(cond)) { error("[%s\tline %5d] ", __FILE__, __LINE__); error(__VA_ARGS__); error(" [aborting]\nSTDERR: %s\n", strerror(errno)); exit(EXIT_FAILURE); } }

#define debug_assert(cond, ...) { if (!(cond)) { error("[%s\tline %5d] ", __FILE__, __LINE__); error("%s\n", strerror(errno)); __VA_ARGS__; } }

#define debug_abort(...) { assert_v(false, __VA_ARGS__); }
