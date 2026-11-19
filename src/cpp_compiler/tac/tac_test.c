#include <stdio.h>
#include "tac.h"

int main(void) {
    const char *source =
        "int i = 0;\n"
        "while (i < 3) {\n"
        "    int j = 0;\n"
        "    while (j < 3) {\n"
        "        j = j + 1;\n"
        "        if (j == 2) {\n"
        "            break;\n"
        "        }\n"
        "    }\n"
        "    i = i + 1;\n"
        "}\n";

    CPPParser parser;
    cpp_parser_init(&parser, source);

    printf("========== NESTED LOOP TAC TEST ==========\n");

    CPPASTNode *root = cpp_parse_program(&parser);

    if (!root || cpp_parser_had_error(&parser)) {
        printf("Syntax Analysis Failed!\n");
        cpp_ast_free(root);
        return 1;
    }

    printf("Syntax Analysis Successful!\n");

    CPPTAC tac;
    cpp_tac_init(&tac);

    int success = cpp_tac_generate(&tac, root);
    cpp_tac_print(&tac);

    printf("\nNested Loop TAC Test %s!\n",
           success ? "Successful" : "Failed");

    cpp_ast_free(root);
    return success ? 0 : 1;
}