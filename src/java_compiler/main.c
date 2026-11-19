
#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "symbol_table/symbol_table.h"
#include "semantic/semantic.h"
#include "tac/tac.h"
#include "optimizer/optimizer.h"

/* Static allocation avoids stack overflow from large structures. */
static Lexer lexer;
static Parser parser;
static SymbolTable symbolTable;
static TACProgram tacProgram;

/* Read source code from a file. */
static char *readSourceFile(const char *filename)
{
    FILE *file;
    long fileSize;
    char *source;

    file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Error: Cannot open file '%s'.\n", filename);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    fileSize = ftell(file);

    if (fileSize < 0) {
        fclose(file);
        return NULL;
    }

    rewind(file);

    source = (char *)malloc((size_t)fileSize + 1);

    if (source == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return NULL;
    }

    if (fread(source, 1, (size_t)fileSize, file) !=
        (size_t)fileSize) {
        printf("Error reading source file.\n");
        free(source);
        fclose(file);
        return NULL;
    }

    source[fileSize] = '\0';

    fclose(file);

    return source;
}

int main(int argc, char *argv[])
{
    char *source;
    ASTNode *root;

    if (argc != 2) {
        printf("Usage: java_compiler <source_file>\n");
        return 1;
    }

    printf("\n========================================\n");
    printf("       JAVA COMPILER\n");
    printf("========================================\n");

    /* Read source file. */
    source = readSourceFile(argv[1]);

    if (source == NULL)
        return 1;

    printf("\nSource File: %s\n", argv[1]);

    /* STEP 1: Lexical Analysis */
    printf("\n========== STEP 1: LEXICAL ANALYSIS ==========\n");

    initLexer(&lexer, source);

    if (!tokenize(&lexer)) {
        printf("Lexical Analysis Failed!\n");
        free(source);
        return 1;
    }

    printf("Lexical Analysis Completed!\n");

    /* STEP 2: Syntax Analysis */
    printf("\n========== STEP 2: SYNTAX ANALYSIS ==========\n");

    initParser(&parser, lexer.tokens, lexer.tokenCount);

    root = parseProgram(&parser);

    if (root == NULL) {
        printf("Syntax Analysis Failed!\n");
        free(source);
        return 1;
    }

    printf("Syntax Analysis Completed!\n");

    /* STEP 3: Semantic Analysis */
    printf("\n========== STEP 3: SEMANTIC ANALYSIS ==========\n");

    initSymbolTable(&symbolTable);

    if (!analyzeSemantics(root, &symbolTable)) {
        printf("Semantic Analysis Failed!\n");

        freeAST(root);
        free(source);

        return 1;
    }

    printf("Semantic Analysis Completed!\n");

    printf("\n========== SYMBOL TABLE ==========\n");
    printSymbolTable(&symbolTable);

    /* STEP 4: Three Address Code Generation */
    printf("\n========== STEP 4: TAC GENERATION ==========\n");

    initTAC(&tacProgram);

    if (!generateTAC(root, &tacProgram)) {
        printf("TAC Generation Failed!\n");

        freeAST(root);
        free(source);

        return 1;
    }

    printf("\n--- GENERATED THREE ADDRESS CODE ---\n");
    printTAC(&tacProgram);

    /* STEP 5: TAC Optimization */
    printf("\n========== STEP 5: TAC OPTIMIZATION ==========\n");

    if (!optimizeTAC(&tacProgram)) {
        printf("TAC Optimization Failed!\n");

        freeAST(root);
        free(source);

        return 1;
    }

    printf("\n--- OPTIMIZED THREE ADDRESS CODE ---\n");
    printOptimizedTAC(&tacProgram);

    printf("\n========================================\n");
    printf("   COMPILATION COMPLETED SUCCESSFULLY!\n");
    printf("========================================\n");

    freeAST(root);
    free(source);

    return 0;
}
