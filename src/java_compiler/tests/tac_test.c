
#include <stdio.h>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../ast/ast.h"
#include "../tac/tac.h"

int main(void)
{
    const char *source =
        "int a = 10;"
        "int b = 20;"
        "int result = a + b;"
        "if (result > 20) {"
        "   print(result);"
        "} else {"
        "   print(0);"
        "}"
        "while (a > 0) {"
        "   a = a - 1;"
        "}";

    static Lexer lexer;
    static Parser parser;
    static TACProgram tac;

    printf("========== TAC GENERATION TEST ==========\n");

    initLexer(&lexer, source);

    if (!tokenize(&lexer)) {
        printf("Lexical Analysis Failed!\n");
        return 1;
    }

    printf("Lexical Analysis Successful!\n");

    initParser(&parser, lexer.tokens, lexer.tokenCount);

    ASTNode *program = parseProgram(&parser);

    if (program == NULL || parser.hasError) {
        printf("Syntax Analysis Failed!\n");
        freeAST(program);
        return 1;
    }

    printf("Syntax Analysis Successful!\n");

    if (!generateTAC(program, &tac)) {
        printf("TAC Generation Failed!\n");
        freeAST(program);
        return 1;
    }

    printTAC(&tac);

    freeAST(program);

    printf("\n========== TEST COMPLETED ==========\n");

    return 0;
}
