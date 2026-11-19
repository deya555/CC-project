#include <stdio.h>
#include "parser.h"

int main(void)
{
    const char *source =
        "int number = 10;\n"
        "if (number > 5) {\n"
        "    number = number + 1;\n"
        "}\n";

    CPPParser parser;
    cpp_parser_init(&parser, source);

    printf("========== C++ PARSER TEST ==========\n");

    CPPASTNode *ast = cpp_parse_program(&parser);

    if (ast != NULL) {
        printf("\n========== ABSTRACT SYNTAX TREE ==========\n");
        cpp_ast_print(ast, 0);
    }

    if (cpp_parser_had_error(&parser)) {
        printf("\nParser test failed. Errors: %d\n", parser.error_count);
    } else {
        printf("\nParser test successful!\n");
    }

    cpp_ast_free(ast);

    return cpp_parser_had_error(&parser) ? 1 : 0;
}
