#include <assert.h>
#include <string.h>
#include "macro.h"

#define ID_SIZE 8
#define SIZE_TYPE int

#define MSG_INDIRECT_VERIFY() MSG_NO_EVAL_VERIFY
#define MSG_NO_EVAL_VERIFY(...) \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  ) \
    ( \
        static_assert(sizeof(STRINGIFY(HEAD1(__VA_ARGS__))) < ID_SIZE, "field id too large: sizeof(" STRINGIFY(HEAD1(__VA_ARGS__)) ") > " STRINGIFY(ID_SIZE) ); \
        DEFER2 ( MSG_INDIRECT_VERIFY ) () (TAIL2(__VA_ARGS__)) \
    )

#define MSG_INDIRECT_DECL() MSG_NO_EVAL_DECL
#define MSG_NO_EVAL_DECL(...)                                                                                     \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  )                                                                      \
    (                                                                                                             \
        char CAT(HEAD1(__VA_ARGS__), _id)[ID_SIZE];                                                               \
        SIZE_TYPE CAT(HEAD1(__VA_ARGS__), _size);                                                                 \
        HEAD(__VA_ARGS__) HEAD1(__VA_ARGS__) [HEAD2(__VA_ARGS__)];                                                \
                                                                                                                  \
        DEFER2 ( MSG_INDIRECT_DECL ) () (TAIL2(__VA_ARGS__))                                                      \
    )

#define MSG_INDIRECT_COMPOUND() MSG_NO_EVAL_COMPOUND
#define MSG_NO_EVAL_COMPOUND(...)                                                                                 \
    IF ( IS_LIST_NOT_EMPTY( __VA_ARGS__ )  )                                                                      \
    (                                                                                                             \
        STRINGIFY(HEAD1(__VA_ARGS__)) COMMA                                                                       \
        sizeof(HEAD(__VA_ARGS__)) * HEAD2(__VA_ARGS__) COMMA                                                      \
        {} COMMA                   \
        DEFER2 ( MSG_INDIRECT_COMPOUND ) () (TAIL2(__VA_ARGS__))                                                  \
    )

#define MSG_DEF(_id, ...)                                                                                  \
    static_assert(sizeof(STRINGIFY(_id)) < ID_SIZE, "id too large: sizeof(" STRINGIFY(_id) ") > " STRINGIFY(ID_SIZE) ); \
    EVAL(MSG_NO_EVAL_VERIFY(__VA_ARGS__))                                                                               \
    struct 
    struct __attribute__((__packed__)) { char id[ID_SIZE]; SIZE_TYPE size; EVAL(MSG_NO_EVAL_DECL(__VA_ARGS__)) }        \
        _id = { STRINGIFY(_id), sizeof(_id), EVAL(MSG_NO_EVAL_COMPOUND(__VA_ARGS__)) }


/*
msg_def(msg, "id", int, a, 1, float, b, 1, char, c, 32) :

struct __attribute__((__packed__))
{
  char id[ID_SIZE] = "msg";
  SIZE_TYPE size = 88;
  char a_id[ID_SIZE] = "a";
  SIZE_TYPE a_size = 4;
  int a[1];
  char b_id[ID_SIZE] = "b";
  SIZE_TYPE b_size = 4;
  float b[1];
  char c_id[ID_SIZE] = "c";
  SIZE_TYPE c_size = 32;
  float c[32];
}
msg =
{
  . a_id = "a" , . a_size = sizeof(int) ,
  . b_id = "b" , . b_size = sizeof(float) ,
};
*/
#define msg_def MSG_DEF

#define msg_match(msg, buffer) (strcmp((typeof(msg)*(buffer)->id), msg.id) == 0)

#define msg_header_size(msg) (sizeof(msg.id) + sizeof(msg.size))

#define msg_size(buffer) (typeof(msg)*(buffer)->msg.size)

// msg_parse(msg, buffer);
#define msg_parse(msg, buffer)                          \
do {                                                    \
  int i = msg_header_size(msg);                         \
  while (i < (msg).size)                                \
  {                                                     \
    void *p;                                            \
    p = (void*)&(msg) + i;                              \
    {                                                   \
      char *field_id = (char*)p;                        \
      /*for (;c[i] != '\0'; ++i) {}*/                   \

      
    }                                                   \
                                                        \
    i += sizeof(msg.id);                                \
    p = (void*)&(msg) + i;                              \
    SIZE_TYPE field_size = *(SIZE_TYPE*)p;              \
                                                        \
    i += sizeof(field_size);                            \
    p = (void*)&(msg) + i;                              \
                                                        \
    switch(field_size)                                  \
    {                                                   \
      case 4:                                           \
        sprintf(ret, "%s, %f", ret, *(float*)p);        \
        break;                                          \
      default:                                          \
        sprintf(ret, "%s, %s", ret, (char*)p);          \
        break;                                          \
    }                                                   \
                                                        \
    i += field_size;                                    \
  }                                                     \
  ret;                                                  \
} while(0)


//// printf(msg_print(msg));
//#define msg_print(msg)                                    \
//  ({                                                      \
//    static char ret[sizeof(msg) * 10];                    \
//    sprintf(ret, "id: %s, size: %d", msg.id, msg.size);   \
//                                                          \
//    int i = sizeof(msg.id) + sizeof(msg.size);            \
//    while (i < (msg).size)                                \
//    {                                                     \
//      void *p;                                            \
//      p = (void*)&(msg) + i;                              \
//      char *c = (char*)p;                                 \
//                                                          \
//      /*for (;c[i] != '\0'; ++i) {}*/                     \
//      sprintf(ret, "%s, %s: ", ret, c);                   \
//                                                          \
//      i += ID_SIZE;                                     \
//      p = (void*)&(msg) + i;                              \
//      SIZE_TYPE field_size = *(SIZE_TYPE*)p;              \
//                                                          \
//      i += sizeof(field_size);                            \
//      p = (void*)&(msg) + i;                              \
//                                                          \
//      switch(field_size)                                  \
//      {                                                   \
//        case 4:                                           \
//          sprintf(ret, "%s, %f", ret, *(float*)p);        \
//          break;                                          \
//        default:                                          \
//          sprintf(ret, "%s, %s", ret, (char*)p);          \
//          break;                                          \
//      }                                                   \
//                                                          \
//      i += field_size;                                    \
//    }                                                     \
//    ret;                                                  \
//  })
