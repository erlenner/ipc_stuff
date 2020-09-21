// macro tutorial: http://saadahmad.ca/cc-preprocessor-metaprogramming-basic-pattern-matching-macros-and-conditionals/

#define EMPTY() 
#define DEFER(...) __VA_ARGS__ EMPTY()
#define DEFER2(...) __VA_ARGS__ DEFER(EMPTY) ()
#define DEFER3(...) __VA_ARGS__ DEFER2(EMPTY) ()
#define DEFER4(...) __VA_ARGS__ DEFER3(EMPTY) ()
#define DEFER5(...) __VA_ARGS__ DEFER4(EMPTY) ()

#define EVAL_1(...) __VA_ARGS__
#define EVAL_2(...) EVAL_1(EVAL_1(__VA_ARGS__))
//#define EVAL_3(...) EVAL_2(EVAL_2(__VA_ARGS__))
//#define EVAL_4(...) EVAL_3(EVAL_3(__VA_ARGS__))
//#define EVAL_5(...) EVAL_4(EVAL_4(__VA_ARGS__))
//#define EVAL_6(...) EVAL_5(EVAL_5(__VA_ARGS__))
//#define EVAL_7(...) EVAL_6(EVAL_6(__VA_ARGS__))
//#define EVAL_8(...) EVAL_7(EVAL_7(__VA_ARGS__))
#define EVAL(...) EVAL_2(__VA_ARGS__)

#define IF_1(true, ...) true
#define IF_0(true, ...) __VA_ARGS__
#define IF(value) CAT(IF_, value)

#define AND_11 EXISTS(1)
#define AND(x, y) TRY_EXTRACT_EXISTS ( CAT(CAT(AND_, x), y), 0 )

#define EAT(...)
#define EXPAND_TEST_EXISTS(...) EXPANDED, EXISTS(__VA_ARGS__) ) EAT (
#define GET_TEST_EXISTS_RESULT(x) ( CAT(EXPAND_TEST_, x),  DOESNT_EXIST )
 
#define GET_TEST_EXIST_VALUE_(expansion, existValue) existValue
#define GET_TEST_EXIST_VALUE(x) GET_TEST_EXIST_VALUE_  x 
 
#define TEST_EXISTS(x) GET_TEST_EXIST_VALUE (  GET_TEST_EXISTS_RESULT(x) )

#define DOES_VALUE_EXIST_EXISTS(...) 1
#define DOES_VALUE_EXIST_DOESNT_EXIST 0
#define DOES_VALUE_EXIST(x) CAT(DOES_VALUE_EXIST_, x)
 
#define EXTRACT_VALUE_EXISTS(...) __VA_ARGS__
#define EXTRACT_VALUE(value) CAT(EXTRACT_VALUE_, value)
 
#define TRY_EXTRACT_EXISTS(value, ...) \
    IF ( DOES_VALUE_EXIST(TEST_EXISTS(value)) )\
    ( EXTRACT_VALUE(value), __VA_ARGS__ )

#define AND_11 EXISTS(1)
#define AND(x, y) TRY_EXTRACT_EXISTS ( CAT(CAT(AND_, x), y), 0 )

#define OR_00 EXISTS(0)
#define OR(x, y) TRY_EXTRACT_EXISTS ( CAT(CAT(OR_, x), y), 1 )

#define XOR_01 EXISTS(1)
#define XOR_10 EXISTS(1)
#define XOR(x, y) TRY_EXTRACT_EXISTS ( CAT(CAT(XOR_, x), y), 0 )

#define NOT_0 EXISTS(1)
#define NOT(x) TRY_EXTRACT_EXISTS ( CAT(NOT_, x), 0 )

#define IS_ZERO_0 EXISTS(1)
#define IS_ZERO(x) TRY_EXTRACT_EXISTS ( CAT(IS_ZERO_, x), 0 )

#define IS_NOT_ZERO(x) NOT ( IS_ZERO(x) )

#define IS_ENCLOSED_TEST(...) EXISTS(1)
#define IS_ENCLOSED(x, ...) TRY_EXTRACT_EXISTS ( IS_ENCLOSED_TEST x, 0 )

#define ENCLOSE_EXPAND(...) EXPANDED, ENCLOSED, (__VA_ARGS__) ) EAT (
#define GET_CAT_EXP(a, b) (a, ENCLOSE_EXPAND b, DEFAULT, b )

#define CAT_WITH_ENCLOSED(a, b) a b
#define CAT_WITH_DEFAULT(a, b) a ## b
#define CAT_WITH(a, _, f, b) CAT_WITH_ ## f (a, b)

#define EVAL_CAT_WITH(...) CAT_WITH __VA_ARGS__
#define CAT(a, b) EVAL_CAT_WITH ( GET_CAT_EXP(a, b) )

#define HEAD(x, ...) x
#define HEAD1(x, y, ...) y
#define HEAD2(x, y, z, ...) z

#define TAIL(x, ...) __VA_ARGS__
#define TAIL1(x, y, ...) __VA_ARGS__
#define TAIL2(x, y, z, ...) __VA_ARGS__
 
#define TEST_LAST EXISTS(1)
#define IS_LIST_EMPTY(...) \
    TRY_EXTRACT_EXISTS( \
        DEFER(HEAD) (__VA_ARGS__ EXISTS(1))\
        , 0)
#define IS_LIST_NOT_EMPTY(...) NOT(IS_LIST_EMPTY(__VA_ARGS__))

#define ENCLOSE(...) ( __VA_ARGS__ )
#define REM_ENCLOSE_(...) __VA_ARGS__
#define REM_ENCLOSE(...) REM_ENCLOSE_ __VA_ARGS__
 
#define IF_ENCLOSED_1(true, ...) true
#define IF_ENCLOSED_0(true, ...) __VA_ARGS__
#define IF_ENCLOSED(...) CAT(IF_ENCLOSED_, IS_ENCLOSED(__VA_ARGS__))
#define OPT_REM_ENCLOSE(...) \
    IF_ENCLOSED (__VA_ARGS__) ( REM_ENCLOSE(__VA_ARGS__), __VA_ARGS__ )
 


#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define COMMA ,

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-fcompare-debug-second"
//
//#pragma GCC diagnostic pop
