
#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"
#include "../semantic/semantic.h"
#include "../tac/tac.h"
#include "../optimizer/optimizer.h"

static Lexer lexer;
static Parser parser;
static SymbolTable symbolTable;
static TACProgram tacProgram;

int main(void)
{
    const char *source =
        "int number = 10;"
        "int result = 20;"
        "result = number + 5;"
        "if (result > 10) {"
        "   result = result + 1;"
        "}"
        "print(result);";

    ASTNode *root;

    printf("\n========== FULL JAVA COMPILER PIPELINE TEST ==========\n");

    /* STEP 1: Lexical Analysis */
    printf("\n========== STEP 1: LEXICAL ANALYSIS ==========\n");

    initLexer(&lexer, source);

    if (!tokenize(&lexer)) {
        printf("Lexical Analysis Failed!\n");
        return 1;
    }

    printf("Lexical Analysis Successful!\n");

    /* STEP 2: Syntax Analysis */
    printf("\n========== STEP 2: SYNTAX ANALYSIS ==========\n");

    initParser(&parser, lexer.tokens, lexer.tokenCount);

    root = parseProgram(&parser);

    if (root == NULL) {
        printf("Syntax Analysis Failed!\n");
        return 1;
    }

    printf("Syntax Analysis Successful!\n");

    /* STEP 3: Semantic Analysis */
    printf("\n========== STEP 3: SEMANTIC ANALYSIS ==========\n");

    initSymbolTable(&symbolTable);

    if (!analyzeSemantics(root, &symbolTable)) {
        printf("Semantic Analysis Failed!\n");
        freeAST(root);
        return 1;
    }

    printf("Semantic Analysis Successful!\n");

    printSymbolTable(&symbolTable);

    /* STEP 4: TAC Generation */
    printf("\n========== STEP 4: TAC GENERATION ==========\n");

    initTAC(&tacProgram);

    if (!generateTAC(root, &tacProgram)) {
        printf("TAC Generation Failed!\n");
        freeAST(root);
        return 1;
    }

    printTAC(&tacProgram);

    /* STEP 5: TAC Optimization */
    printf("\n========== STEP 5: TAC OPTIMIZATION ==========\n");

    if (!optimizeTAC(&tacProgram)) {
        printf("TAC Optimization Failed!\n");
        freeAST(root);
        return 1;
    }

    printOptimizedTAC(&tacProgram);

    printf("\n========== FULL PIPELINE TEST COMPLETED ==========\n");

    freeAST(root);

    return 0;
}
