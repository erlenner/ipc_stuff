#include <stdint.h>
#include "macro.h"

#define NAME_SIZE 8
#define ID_SIZE 8
#define SIZE_TYPE int

#define MSG_INDIRECT_VERIFY() MSG_NO_EVAL_VERIFY
#define MSG_NO_EVAL_VERIFY(...) \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  ) \
    ( \
        static_assert(sizeof(STRINGIFY(HEAD(__VA_ARGS__))) < NAME_SIZE, "field name too large: sizeof(" STRINGIFY(HEAD1(__VA_ARGS__)) ") > " STRINGIFY(NAME_SIZE) ); \
        DEFER2 ( MSG_INDIRECT_VERIFY ) () (TAIL2(__VA_ARGS__)) \
    )

#define MSG_INDIRECT_DECL() MSG_NO_EVAL_DECL
#define MSG_NO_EVAL_DECL(...)                                                                                     \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  )                                                                      \
    (                                                                                                             \
        char CAT(HEAD1(__VA_ARGS__), _name)[NAME_SIZE];                                                           \
        SIZE_TYPE CAT(HEAD1(__VA_ARGS__), _size);                                                                 \
        HEAD(__VA_ARGS__) HEAD1(__VA_ARGS__) [HEAD2(__VA_ARGS__)];                                                \
                                                                                                                  \
        DEFER2 ( MSG_INDIRECT_DECL ) () (TAIL2(__VA_ARGS__))                                                      \
    )

#define MSG_INDIRECT_COMPOUND() MSG_NO_EVAL_COMPOUND
#define MSG_NO_EVAL_COMPOUND(...)                                                                                 \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  )                                                                      \
    (                                                                                                             \
        . CAT(HEAD1(__VA_ARGS__), _name) = STRINGIFY(HEAD1(__VA_ARGS__)) COMMA                                    \
        . CAT(HEAD1(__VA_ARGS__), _size) = sizeof(HEAD(__VA_ARGS__)) COMMA                                        \
        /*.HEAD1(__VA_ARGS__) = 0;*/                                                                              \
        DEFER2 ( MSG_INDIRECT_COMPOUND ) () (TAIL2(__VA_ARGS__))                                                  \
    )

#define MSG_DEF(_name, _id, ...)                                                                                  \
    static_assert(sizeof(_id) < ID_SIZE, "id too large: sizeof(" _id ") > " STRINGIFY(ID_SIZE) );                 \
    EVAL(MSG_NO_EVAL_VERIFY(__VA_ARGS__))                                                                         \
    struct __attribute__((__packed__)) { char id[ID_SIZE]; SIZE_TYPE size; EVAL(MSG_NO_EVAL_DECL(__VA_ARGS__)) }  \
        _name = { .id = _id, .size = sizeof(_name), EVAL(MSG_NO_EVAL_COMPOUND(__VA_ARGS__)) }


/*
msg_def(msg, 'a', int, a, 1, float, b, 1, char, c, 32) :

struct __attribute__((__packed__))
{
  char id[ID_SIZE];
  SIZE_TYPE size;
  char a_name[NAME_SIZE];
  SIZE_TYPE a_size;
  int a[1];
  char b_name[NAME_SIZE];
  SIZE_TYPE b_size;
  float b[1];
  char c_name[NAME_SIZE];
  SIZE_TYPE c_size;
  float c[32];
}
msg =
{
  . a_name = "a" , . a_size = sizeof(int) ,
  . b_name = "b" , . b_size = sizeof(float) ,
};
*/
#define msg_def MSG_DEF
//
//
//#define TYPE_FROM_SIZE(x) 
//  _Generic((x),                              \
//    char:  (char)0,                          \
//    __scalar_type_to_expr_cases(char),       \
//    __scalar_type_to_expr_cases(short),      \
//    __scalar_type_to_expr_cases(int),        \
//    __scalar_type_to_expr_cases(long),       \
//    __scalar_type_to_expr_cases(long long),  \
//    default: (x)))
//
//// printf(msg_print(msg));
//#define msg_print(msg)\
//  ({
//    static char ret[(msg).size];
//    sprintf(ret, "id: %s, size: %d", msg.id, msg.size);
//
//    int i = sizeof(msg.id) + sizeof(msg.size);
//    while (i < (msg).size)
//    {
//      char *c = ((char*)&(msg)) + i;
//      
//      //for (;c[i] != '\0'; ++i) {}
//      sprintf(ret, "%s, %s: %f", ret, c, (float)msg);
//
//      sprintf(ret, "%s ")
//    }
//    ret;
//  })
