#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helper.h"

#define Qfalse 0x00
#define Qnil   0x04
#define Qtrue  0x14
#define Qundef 0x24

#define rb_encoding void
#define OnigCodePoint unsigned int

ID next_id = 0;

static Number *
number_new(void)
{
    Number *number;

    number = (Number *)malloc(sizeof(Number));
    number->type = T_NUMBER;

    return number;
}

static Number *
number_new_with_long(long l)
{
    Number *number = number_new();
    number->l = l;

    return number;
}

static String *
string_new(void)
{
    String *string;

    string = (String *)malloc(sizeof(String));
    string->type = T_STRING;

    return string;
}

static String *
string_new_with_str_len(const char *ptr, long len)
{
    String *string = string_new();
    string->s = malloc(sizeof(char) * len);
    memcpy(string->s, ptr, len);
    string->len = len;

    return string;
}

String *
string_new_with_str(const char *ptr)
{
    return string_new_with_str_len(ptr, strlen(ptr));
}

static Hash *
hash_new(void)
{
    Hash *hash;

    hash = (Hash *)malloc(sizeof(Hash));
    hash->type = T_HASH;

    return hash;
}

static IO *
io_new(void)
{
    IO *io;

    io = (IO *)malloc(sizeof(IO));
    io->type = T_IO;

    return io;
}

static Encoding *
encoding_new(void)
{
    Encoding *encoding;

    encoding = (Encoding *)malloc(sizeof(Encoding));
    encoding->type = T_ENCODING;

    return encoding;
}

static VALUE
debug_output_stdout(void)
{
    return (VALUE)io_new();
}

static rb_encoding *
utf8_encoding(void)
{
    return (rb_encoding *)encoding_new();
}

static VALUE
filesystem_str_new_cstr(const char *ptr)
{
    return (VALUE)string_new_with_str(ptr);
}

static rb_encoding *
enc_get(VALUE obj)
{
    return utf8_encoding();
}

static int
enc_asciicompat(rb_encoding *enc)
{
    return 1;
}

static VALUE
str_new_frozen(VALUE orig)
{
    return orig;
}

static int
nil_p(VALUE obj)
{
    return obj == Qnil;
}

static VALUE
fstring(VALUE obj)
{
    return obj;
}

static char *
string_value_cstr(volatile VALUE *ptr)
{
    return ((String *)*ptr)->s;
}

static rb_ast_t *
ast_new(VALUE nb)
{
    rb_ast_t *ast;

    ast = (rb_ast_t *)malloc(sizeof(rb_ast_t));
    ast->node_buffer = (node_buffer_t *)nb;

    return ast;
}

static VALUE
compile_callback(VALUE (*func)(VALUE), VALUE arg)
{
    return func(arg);
}

static char *
rstring_ptr(VALUE str)
{
    return ((String *)str)->s;
}

static long
rstring_len(VALUE str)
{
    return ((String *)str)->len;
}

static VALUE
str_subseq(VALUE str, long beg, long len)
{
    return (VALUE)string_new_with_str_len(rstring_ptr(str) + beg, len);
}

static int
obj_frozen(VALUE obj)
{
    return 0;
}

static VALUE
verbose(void)
{
    return Qfalse;
}

static int
rtest(VALUE obj)
{
    return obj != Qfalse;
}

static void *
alloc_n(size_t nelems, size_t elemsiz)
{
    return malloc(nelems * elemsiz);
}

static int
isupper(int c)
{
    return 'A' <= c && c <= 'Z';
}

static int
islower(int c)
{
    return 'a' <= c && c <= 'z';
}

static int
isdigit(int c)
{
    return '0' <= c && c <= '9';
}

static int
isalpha(int c)
{
    return isupper(c) || islower(c);
}

static int
isalnum(int c)
{
    return isalpha(c) || isdigit(c);
}

static int
enc_isalnum(OnigCodePoint c, rb_encoding *enc)
{
    return isalnum(c);
}

static int
enc_precise_mbclen(const char *p, const char *e, rb_encoding *enc)
{
    return 1;
}

static int
mbclen_charfound_p(int len)
{
    return 1;
}

static ID
id_new(void)
{
    ID id = next_id;
    next_id++;
    return id;
}

static ID
intern3(const char *name, long len, rb_encoding *enc)
{
    return id_new();
}

static int
is_local_id(ID id)
{
    return 0;
}

static VALUE
cstr_to_inum(const char *str, int base, int badcheck)
{
    return (VALUE)number_new_with_long((long)atoi(str));
}

static VALUE
obj_written(VALUE old, VALUE slot, VALUE young)
{
    return old;
}

static void
sized_xfree(void *x, size_t size)
{
    free(x);
}

static VALUE
ident_hash_new(void)
{
    return (VALUE)hash_new();
}

static VALUE
obj_hide(VALUE obj)
{
    return obj;
}

static VALUE
sym_intern_ascii_cstr(const char *ptr)
{
    return id_new();
}

static VALUE
hash_aset(VALUE hash, VALUE key, VALUE val)
{
    return val;
}

static VALUE
obj_write(VALUE old, VALUE *slot, VALUE young)
{
    return old;
}

static VALUE
int2fix(long i)
{
    return (VALUE)number_new_with_long(i);
}

static void
print_literal(FILE *fp, VALUE lit)
{
    object_type_t type = ((Object *)lit)->type;

    switch (type) {
      case T_NUMBER:
        fprintf(fp, "(number) %ld", ((Number *)lit)->l);
        return;
      case T_STRING:
        fprintf(fp, "(string)");
        return;
      case T_HASH:
        fprintf(fp, "(hash)");
        return;
      case T_IO:
        fprintf(fp, "(io)");
        return;
      case T_ENCODING:
        fprintf(fp, "(encoding)");
        return;
      default:
        fprintf(fp, "(Unknown) %i", type);
        return;
    }
}

static void
print_indent(FILE *fp, int indent)
{
    for (int i = 0; i < indent; i++) {
        fprintf(fp, "    ");
    }
}

#define PRINT(io, str) \
    do { \
        print_indent(io, indent); \
        fprintf(io, str); \
    } while (0)

#define FPRINTF(io, fmt, ...) \
    do { \
        print_indent(io, indent); \
        fprintf(io, fmt, __VA_ARGS__); \
    } while (0)

#define PRINT_ID(io, id) \
    do { \
        if (id < 128) { \
            FPRINTF(io, "id: %c\n", (char)id); \
        } \
        else { \
            FPRINTF(io, "id: %lu\n", id); \
        } \
    } while (0)

#define PRINT_LIT(io, lit) \
    do { \
        print_indent(io, indent); \
        fprintf(io, "lit: "); \
        print_literal(io, lit); \
        fprintf(io, "\n"); \
    } while (0)

static void dump_node(const NODE *node, const int indent);

static void
print_array(FILE *fp, int indent, const NODE *node)
{
    dump_node(RNODE_LIST(node)->nd_head, indent+1);
    while (RNODE_LIST(node)->nd_next && nd_type(RNODE_LIST(node)->nd_next) == NODE_LIST) {
        node = RNODE_LIST(node)->nd_next;
        dump_node(RNODE_LIST(node)->nd_head, indent+1);
    }
}

static void
dump_node(const NODE *node, const int indent)
{
    enum node_type type;

    if (!node) {
        return;
    }

    type = nd_type(node);
    switch (type) {
      case NODE_SCOPE:
        PRINT(stderr, "SCOPE:\n");
        // dump_node(RNODE_SCOPE(node)->nd_args, indent+1);
        dump_node(RNODE_SCOPE(node)->nd_body, indent+1);
        return;
      case NODE_OPCALL:
        PRINT(stderr, "OPCALL:\n");
        PRINT_ID(stderr, RNODE_OPCALL(node)->nd_mid);
        dump_node(RNODE_OPCALL(node)->nd_recv, indent+1);
        dump_node(RNODE_OPCALL(node)->nd_args, indent+1);
        return;
      case NODE_LIT:
        PRINT(stderr, "LIT:\n");
        PRINT_LIT(stderr, RNODE_LIT(node)->nd_lit);
        return;
      case NODE_LIST:
        PRINT(stderr, "LIST:\n");
        print_array(stderr, indent, node);
        return;
      default:
        FPRINTF(stderr, "Unknown type: %i\n", type);
        return;
    }
}

void
parser_config_initialize(rb_parser_config_t *config)
{
    config->calloc = calloc;
    config->malloc = malloc;
    config->alloc = malloc;
    config->alloc_n = alloc_n;
    config->sized_xfree = sized_xfree;

    config->qnil = Qnil;
    config->qtrue = Qtrue;
    config->qfalse = Qfalse;
    config->qundef = Qundef;

    config->debug_output_stdout = debug_output_stdout;
    config->debug_output_stderr = debug_output_stdout;
    // config->io_write = ;
    // config->io_flush = ;
    config->utf8_encoding = utf8_encoding;
    config->filesystem_str_new_cstr = filesystem_str_new_cstr;
    config->enc_get = enc_get;
    config->enc_asciicompat = enc_asciicompat;
    config->str_new_frozen = str_new_frozen;
    config->nil_p = nil_p;
    config->fstring = fstring;
    config->string_value_cstr = string_value_cstr;
    config->ast_new = ast_new;
    config->compile_callback = compile_callback;
    config->rstring_ptr = rstring_ptr;
    config->rstring_len = rstring_len;
    config->str_subseq = str_subseq;
    config->obj_frozen = obj_frozen;
    config->verbose = verbose;
    config->rtest = rtest;
    config->enc_isalnum = enc_isalnum;
    config->enc_precise_mbclen = enc_precise_mbclen;
    config->mbclen_charfound_p = mbclen_charfound_p;
    config->intern3 = intern3;
    config->is_local_id = is_local_id;
    config->cstr_to_inum = cstr_to_inum;
    config->obj_written = obj_written;
    config->ident_hash_new = ident_hash_new;
    config->obj_hide = obj_hide;
    config->sym_intern_ascii_cstr = sym_intern_ascii_cstr;
    config->hash_aset = hash_aset;
    config->obj_write = obj_write;
    config->int2fix = int2fix;
}

void
dump_ast(rb_ast_t *ast)
{
    dump_node(ast->body.root, 0);
}

/* For test */
#ifdef RUBY_PARSER_TEST
#include "ruby/internal/method.h"
#include "ruby/internal/module.h"
static VALUE rb_mRubyParser;

static const char *script = "1 + 2";

static VALUE
test(VALUE self)
{
    rb_parser_config_t *config;
    rb_parser_t *parser;
    String *string;
    rb_ast_t *ast;

    string = string_new_with_str(script);

    config = (rb_parser_config_t *)malloc(sizeof(rb_parser_config_t));
    parser_config_initialize(config);
    parser = rb_ruby_parser_new(config);
    ast = rb_ruby_parser_compile_string(parser, "main", (VALUE)string, 0);
    dump_ast(ast);

    return Qtrue;
}

void
Init_ruby_parser(void)
{
    rb_mRubyParser = rb_define_module("RubyParser");
    rb_define_module_function(rb_mRubyParser, "test", test, 0);
}
#endif /* RUBY_PARSER_TEST */
