#include "helper.h"

static const char *script = "1 + 2";

int
main()
{
    rb_parser_config_t *config;
    rb_parser_t *parser;
    String *string;
    rb_ast_t *ast;

    string = string_new_with_str(script);

    config = (rb_parser_config_t *)malloc(sizeof(rb_parser_config_t));
    // rb_parser_config_default_initialize(config);
    parser_config_initialize(config);
    parser = rb_ruby_parser_new(config);
    ast = rb_ruby_parser_compile_string(parser, "main", (VALUE)string, 0);
    dump_ast(ast);
}
