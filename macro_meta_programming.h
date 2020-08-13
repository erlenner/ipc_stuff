#pragma once

#define EMPTY() 
#define DEFER(...) __VA_ARGS__ EMPTY()
#define DEFER2(...) __VA_ARGS__ DEFER(EMPTY) ()
#define DEFER3(...) __VA_ARGS__ DEFER2(EMPTY) ()
#define DEFER4(...) __VA_ARGS__ DEFER3(EMPTY) ()
#define DEFER5(...) __VA_ARGS__ DEFER4(EMPTY) ()

#define EVAL1(...) __VA_ARGS__
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL3(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL4(...) EVAL3(EVAL3(__VA_ARGS__))
#define EVAL5(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL6(...) EVAL5(EVAL5(__VA_ARGS__))
#define EVAL7(...) EVAL6(EVAL6(__VA_ARGS__))
#define EVAL8(...) EVAL7(EVAL7(__VA_ARGS__))
#define EVAL(...) EVAL8(__VA_ARGS__)

// Concatenation
#define APPLY_CAT(a, ...) a ## __VA_ARGS__
#define CAT2(a,...) APPLY_CAT(a,__VA_ARGS__)
#define CAT3(a,...) EVAL(CAT2(a,CAT2(__VA_ARGS__)))
#define CAT4(a,...) EVAL(CAT2(a,CAT3(__VA_ARGS__)))
#define CAT5(a,...) EVAL(CAT2(a,CAT4(__VA_ARGS__)))
#define CAT6(a,...) EVAL(CAT2(a,CAT5(__VA_ARGS__)))
#define CAT7(a,...) EVAL(CAT2(a,CAT6(__VA_ARGS__)))
