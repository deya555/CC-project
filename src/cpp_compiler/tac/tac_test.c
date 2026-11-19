
#include <stdio.h>

#include "tac.h"

int main(void) {
    const char *source =
        "int a = 10;\n"
        "int b = 20;\n"
        "int result = a + b * 2;\n";

    CPPParser parser;
    cpp_parser_init(&parser, source);

    printf("========== C++ TAC GENERATION TEST ==========\n");

    CPPASTNode *root = cpp_parse_program(&parser);

    if (!root || cpp_parser_had_error(&parser)) {
        printf("Parser failed. TAC generation cannot continue.\n");
        cpp_ast_free(root);
        return 1;
    }

    CPPTAC tac;
    cpp_tac_init(&tac);

    printf("\nSTEP 1: THREE ADDRESS CODE GENERATION\n");

    int success = cpp_tac_generate(&tac, root);

    cpp_tac_print(&tac);

    if (success)
        printf("\nTAC generation successful!\n");
    else
        printf("\nTAC generation failed!\n");

    cpp_ast_free(root);

    return success ? 0 : 1;
}
