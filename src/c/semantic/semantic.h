#ifndef C_SEMANTIC_H
#define C_SEMANTIC_H

#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"

// Tracks the number of semantic errors
extern int semantic_error_count;

// Perform semantic analysis on the AST
void semantic_analyze(ASTNode *root, SymbolTable *table);

#endif