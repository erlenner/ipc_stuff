#pragma once

// for parameters in function calls
#define COMMA(...) , ##__VA_ARGS__

// http://saadahmad.ca/cc-preprocessor-metaprogramming-evaluating-and-defering-macro-calls/
#define EVAL(...) __VA_ARGS__

// Concatenation
#define APPLY_CAT(a, ...) a ## __VA_ARGS__
#define CAT2(a,...) APPLY_CAT(a,__VA_ARGS__)
#define CAT3(a,...) EVAL(CAT2(a,CAT2(__VA_ARGS__)))
#define CAT4(a,...) EVAL(CAT2(a,CAT3(__VA_ARGS__)))
#define CAT5(a,...) EVAL(CAT2(a,CAT4(__VA_ARGS__)))
#define CAT6(a,...) EVAL(CAT2(a,CAT5(__VA_ARGS__)))
#define CAT7(a,...) EVAL(CAT2(a,CAT6(__VA_ARGS__)))
