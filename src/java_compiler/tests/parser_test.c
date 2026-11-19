
#include <stdio.h>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../ast/ast.h"

int main(void)
{
    const char *source =
        "int x = 10;"
        "boolean flag = true;"
        "if (x >= 5 && flag == true) {"
        "    print(x);"
        "} else {"
        "    print(0);"
        "}"
        "while (x > 0) {"
        "    x = x - 1;"
        "}";

    static Lexer lexer;
    static Parser parser;

    printf("========== JAVA PARSER TEST ==========\n\n");

    /* Step 1: Lexical Analysis */
    initLexer(&lexer, source);

    if (!tokenize(&lexer)) {
        printf("Lexical Analysis Failed!\n");
        return 1;
    }

    printf("Lexical Analysis Successful!\n");

    /* Step 2: Syntax Analysis */
    initParser(&parser, lexer.tokens, lexer.tokenCount);

    ASTNode *program = parseProgram(&parser);

    if (program == NULL || parser.hasError) {
        printf("Syntax Analysis Failed!\n");
        freeAST(program);
        return 1;
    }

    printf("Syntax Analysis Successful!\n\n");

    /* Step 3: Print AST */
    printf("========== ABSTRACT SYNTAX TREE ==========\n");

    printAST(program, 0);

    freeAST(program);

    printf("\n========== TEST COMPLETED ==========\n");

    return 0;
}
