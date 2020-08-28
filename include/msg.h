#include <stdint.h>
#include "macro.h"

#define NAME_SIZE 8
#define ID_SIZE 8
#define SIZE_TYPE int

#define MSG_INDIRECT_VERIFY() MSG_NO_EVAL_VERIFY
#define MSG_NO_EVAL_VERIFY(...) \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  ) \
    ( \
        static_assert(sizeof(STRINGIFY(VICE(__VA_ARGS__))) < NAME_SIZE, "field name too large: sizeof(" STRINGIFY(VICE(__VA_ARGS__)) ") > " STRINGIFY(NAME_SIZE) ); \
        DEFER2 ( MSG_INDIRECT_VERIFY ) () (VICE_TAIL(__VA_ARGS__)) \
    )

#define MSG_INDIRECT_DECL() MSG_NO_EVAL_DECL
#define MSG_NO_EVAL_DECL(...)                                                                                     \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  )                                                                      \
    (                                                                                                             \
        char CAT(VICE(__VA_ARGS__), _name)[NAME_SIZE];                                                            \
        SIZE_TYPE CAT(VICE(__VA_ARGS__), _size);                                                                  \
        HEAD(__VA_ARGS__) VICE(__VA_ARGS__);                                                                      \
                                                                                                                  \
        DEFER2 ( MSG_INDIRECT_DECL ) () (VICE_TAIL(__VA_ARGS__))                                                  \
    )

#define MSG_INDIRECT_COMPOUND() MSG_NO_EVAL_COMPOUND
#define MSG_NO_EVAL_COMPOUND(...)                                                                                 \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  )                                                                      \
    (                                                                                                             \
        . CAT(VICE(__VA_ARGS__), _name) = STRINGIFY(VICE(__VA_ARGS__)) COMMA                                      \
        . CAT(VICE(__VA_ARGS__), _size) = sizeof(HEAD(__VA_ARGS__)) COMMA                                         \
        /*.VICE(__VA_ARGS__) = 0;*/                                                                               \
        DEFER2 ( MSG_INDIRECT_COMPOUND ) () (VICE_TAIL(__VA_ARGS__))                                              \
    )

#define MSG_DEF(_name, _id, ...)                                                                                  \
    static_assert(sizeof(_id) < ID_SIZE, "id too large: sizeof(" _id ") > " STRINGIFY(ID_SIZE) );                 \
    EVAL(MSG_NO_EVAL_VERIFY(__VA_ARGS__))                                                                         \
    struct __attribute__((__packed__)) { char id[ID_SIZE]; SIZE_TYPE size; EVAL(MSG_NO_EVAL_DECL(__VA_ARGS__)) }  \
        _name = { .id = _id, .size = sizeof(_name), EVAL(MSG_NO_EVAL_COMPOUND(__VA_ARGS__)) }


/*
msg_def(msg, 'a', int, a, float, b) :

struct __attribute__((__packed__))
{
  char id[ID_SIZE];
  SIZE_TYPE size;
  char a_name[NAME_SIZE];
  SIZE_TYPE a_size;
  int a;
  char b_name[NAME_SIZE];
  SIZE_TYPE b_size;
  float b;
}
msg =
{
  . a_name = "a" , . a_size = sizeof(int) ,
  . b_name = "b" , . b_size = sizeof(float) ,
};
*/
#define msg_def MSG_DEF
