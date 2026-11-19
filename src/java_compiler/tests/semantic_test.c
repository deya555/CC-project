
#include <stdio.h>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"
#include "../semantic/semantic.h"

static int runTest(const char *testName, const char *source)
{
    static Lexer lexer;
    static Parser parser;
    static SymbolTable table;

    printf("\n----------------------------------------\n");
    printf("TEST: %s\n", testName);
    printf("----------------------------------------\n");

    initLexer(&lexer, source);

    if (!tokenize(&lexer)) {
        printf("Lexical Analysis Failed!\n");
        return 0;
    }

    initParser(&parser, lexer.tokens, lexer.tokenCount);

    if (parser.hasError) {
        printf("Parser initialization failed!\n");
        return 0;
    }

    ASTNode *program = parseProgram(&parser);

    if (program == NULL || parser.hasError) {
        printf("Syntax Analysis Failed!\n");
        freeAST(program);
        return 0;
    }

    printf("Syntax Analysis Successful!\n");

    initSymbolTable(&table);

    int result = analyzeSemantics(program, &table);

    printSymbolTable(&table);

    freeAST(program);

    return result;
}

int main(void)
{
    printf("========== SEMANTIC ANALYZER TEST ==========\n");

    int passed = 0;
    int total = 3;

    /* Test 1: Valid program */
    if (runTest(
        "Valid Program",
        "int number = 10;"
        "int result = number + 5;"
        "if (result > 10) { print(result); }"
    )) {
        passed++;
    }

    /* Test 2: Undeclared variable */
    if (!runTest(
        "Undeclared Variable",
        "int number = 10;"
        "result = number + 5;"
    )) {
        passed++;
    }

    /* Test 3: Type mismatch */
    if (!runTest(
        "Type Mismatch",
        "int number = true;"
    )) {
        passed++;
    }

    printf("\n========== TEST SUMMARY ==========\n");
    printf("Passed: %d/%d\n", passed, total);

    if (passed == total) {
        printf("All semantic tests passed!\n");
        return 0;
    }

    printf("Some semantic tests failed!\n");
    return 1;
}
