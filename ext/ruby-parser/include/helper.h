#ifndef HELPER_H
#define HELPER_H 1

#include "node.h"

typedef enum object_type {
    T_NUMBER,
    T_STRING,
    T_HASH,
    T_IO,
    T_ENCODING,
} object_type_t;

typedef struct Object {
    object_type_t type;    
} Object;

typedef struct Number {
    object_type_t type;
    long l;
} Number;

typedef struct String {
    object_type_t type;
    char *s;
    size_t len;
} String;

typedef struct Hash {
    object_type_t type;
} Hash;

typedef struct IO {
    object_type_t type;
} IO;

typedef struct Encoding {
    object_type_t type;
} Encoding;

String *string_new_with_str(const char *ptr);
void parser_config_initialize(rb_parser_config_t *config);
void dump_ast(rb_ast_t *ast);

#endif /* HELPER_H */
