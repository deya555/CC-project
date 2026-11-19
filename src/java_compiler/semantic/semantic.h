
#ifndef JAVA_SEMANTIC_H
#define JAVA_SEMANTIC_H

#include "../parser/parser.h"

#define MAX_SYMBOLS 500

typedef struct {
    char name[MAX_LEXEME];
    char type[MAX_LEXEME];
    int initialized;
    int line;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
    int errorCount;
} SemanticAnalyzer;

void initSemantic(SemanticAnalyzer *analyzer);

int analyzeSemantics(SemanticAnalyzer *analyzer, ASTNode *root);

void printSymbolTable(SemanticAnalyzer *analyzer);

#endif
