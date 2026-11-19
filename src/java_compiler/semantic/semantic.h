
#ifndef JAVA_SEMANTIC_H
#define JAVA_SEMANTIC_H

#include "../parser/parser.h"

#define MAX_SYMBOLS 500
#define MAX_SCOPES 200

typedef struct {
    char name[MAX_LEXEME];
    char type[MAX_LEXEME];
    int initialized;
    int line;
    int scopeId;
} Symbol;

typedef struct {
    int id;
    int parentId;
} Scope;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    Scope scopes[MAX_SCOPES];

    int count;
    int errorCount;

    int scopeCount;
    int currentScope;
} SemanticAnalyzer;

void initSemantic(SemanticAnalyzer *analyzer);

int analyzeSemantics(SemanticAnalyzer *analyzer, ASTNode *root);

void printSymbolTable(SemanticAnalyzer *analyzer);

#endif
