
#include <stdio.h>

#include "semantic.h"

int main(void) {
    const char *source =
        "int number = 10;\n"
        "int result = 20;\n"
        "result = number + 5;\n";

    CPPParser parser;
    cpp_parser_init(&parser, source);

    CPPASTNode *root = cpp_parse_program(&parser);

    printf("========== C++ SEMANTIC ANALYSIS TEST ==========\n");

    if (!root || cpp_parser_had_error(&parser)) {
        printf("Parser failed. Semantic analysis cannot continue.\n");
        cpp_ast_free(root);
        return 1;
    }

    CPPSemanticAnalyzer analyzer;
    cpp_semantic_init(&analyzer);

    int success = cpp_semantic_analyze(&analyzer, root);

    printf("\nSTEP 1: SEMANTIC ANALYSIS\n");

    cpp_semantic_print_errors(&analyzer);

    if (success)
        printf("\nSemantic test successful!\n");
    else
        printf("\nSemantic test failed!\n");

    cpp_ast_free(root);

    return success ? 0 : 1;
}
